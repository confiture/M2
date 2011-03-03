#include "MedicalViewer.h"

MedicalViewer::MedicalViewer(QString filename, QWidget *parent) : QMainWindow(parent) {
	this->filename = filename;

	// Mise en forme générale (effectuée par le fichier ui dessiné sous QtDesigner).
	ui.setupUi(this);
	initWindow();


	// Lecture du fichier image
	if ( readVolumeImage(filename.toAscii()) ) {
		// Mettre à jour les infos sur l'image dans l'interface graphique
		setImageInfo();
		// Initialisation de l'histogramme
		initHisto();
		// Affichage de l'histogramme
		displayHisto();
		// Initialisation de la lookup table
		initLookupTable();
		// Création des briques du pipeline pour la coupe axial
		initSliceTools();

		initRayCast();

		this->isoVal=0;
		initIsoSurface();
	}

	// On met tout à jour...
	// Place automatiquement la caméra correctement
	//ren->Render();

	// Place correctement le tout dans la fenêtre générale.
	ui.rendererLayout->update();
}

void MedicalViewer::initWindow()
{
	// Création de la fenêtre 3D VTK
	// un widget (le contour et les interactions utilisateur (souris, etc.)
	vtkWidget = new QVTKWidget(this,QFlag(0));

	// un renderer qui va afficher les différents objets
	ren = vtkSmartPointer<vtkRenderer>::New();
	ren->SetBackground(1.0,1.0,1.0);

	// On ajoute le renderer au widget.
	vtkWidget->GetRenderWindow()->AddRenderer(ren);

	// On met le widget Vtk au bon endroit dans la fenêtre Qt(dans un layout nommé rendererLayout)
	ui.rendererLayout->addWidget(vtkWidget);

}

void MedicalViewer::initHisto(){
	vtkHistoWidget = new QVTKWidget(this,QFlag(0));


	// un renderer qui va afficher les différents objets
	renHisto = vtkSmartPointer<vtkRenderer>::New();
	//renHisto->SetBackground(1.0,1.0,1.0);

	// On ajoute le renderer au widget.
	vtkHistoWidget->GetRenderWindow()->AddRenderer(renHisto);

	// On met le widget Vtk au bon endroit dans la fenêtre Qt(dans un layout nommé rendererLayout)
	ui.HistoLayout->addWidget(vtkHistoWidget);

	ui.HistoLayout->update();
}


MedicalViewer::~MedicalViewer()
{
	ren->Delete();
	delete vtkWidget;
}


bool MedicalViewer::readVolumeImage(const char * filename)
{
	// Le vtkMetaImageReader permet de lire les formats .mha et .mhd
	reader = vtkMetaImageReader::New();
	// Image temporaire qui va servir à stocker la sortie du reader
	vtkImageData * tmpImage;

	// Lorsqu'un reader n'arrive pas à lire un fichier (fichier inexistant ou corrompu)
	// il peut renvoyer une exception...
	try {
		// Dans le pipeline Vtk, l'entrée est le nom du fichier...
		reader->SetFileName(filename);

		// ... et la sortie est l'image temporaire.
		tmpImage = reader->GetOutput();

		// En général, une brique du pipeline vtk ne se met à jour que lorsque c'est
		// nécessaire (pour l'affichage par exemple).
		// Ici, on souhaite récupérer l'image lue, avant l'affichage et la stocker
		// dans l'attribut volumeImage. On utilise donc update pour effectuer la lecture
		// avant de passer à la suite. Cela permet aussi de vérifier à ce moment là
		// (et non plus tard) que le fichier image est correct.
		reader->Update();
	} catch(...) {
		QString error;
		cout << "VTK was unable to read file " << filename << endl;
		volumeImage = NULL;
		return false;
	}

	// Une fois lue correctement, on garde l'image dans l'attribut volumeImage.
	volumeImage = vtkImageData::New();
	// On doit faire une DeepCopy, car une fois le reader détruit, sa sortie,
	// c'est-à-dire l'image temporaire sera aussi détruite.
	volumeImage->DeepCopy(tmpImage);


	// Par la suite, on n'a plus besoin du reader (on ne lit qu'un fichier à la fois pour l'instant).
	// mais si... on en a besoin
	//reader->Delete();

	return true;
}


void MedicalViewer::setImageInfo()
{
	/* *************** Récupérer les dimensions de l'image ***********************/
	// On récupère les dimensions de l'image
	volumeImage->GetWholeExtent(extent);
	// On récupère la taille des voxels
	volumeImage->GetSpacing(spacing);
	// On récupère l'origine de l'image
	volumeImage->GetOrigin(origin);

	// A partir des données précédentes, on calcule le centre de l'image
	center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
	center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
	center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

	/* *************** Mettre à jour l'interface à partir des données image *******/
	QString tmpMsg;
	tmpMsg.sprintf("%dx%dx%d", (extent[0] + extent[1])+1, (extent[2] + extent[3])+1,(extent[4] + extent[5])+1); // A changer !
	ui.imageSizeValue->setText(tmpMsg);
	tmpMsg.sprintf("%.2fx%.2fx%.2f", spacing[0], spacing[1], spacing[2]);
	ui.voxelSizeValue->setText(tmpMsg);

	// On peut aussi mettre à jour les spinBox/sliders qui permet de déterminer
	// le numéro du slice axial.
	ui.axialSliceNumberSpinBox->setMinimum(extent[4]);
	ui.axialSliceNumberSpinBox->setMaximum(extent[5]);
	ui.axialSliceNumberSlider->setMinimum(extent[4]);
	ui.axialSliceNumberSlider->setMaximum(extent[5]);

	// le numéro du slice coronal.
	ui.coronalSliceNumberSpinBox->setMinimum(extent[0]);
	ui.coronalSliceNumberSpinBox->setMaximum(extent[1]);
	ui.coronalSliceNumberSlider->setMinimum(extent[0]);
	ui.coronalSliceNumberSlider->setMaximum(extent[1]);

	// le numéro du slice Sagittal.
	ui.sagittalSliceNumberSpinBox->setMinimum(extent[2]);
	ui.sagittalSliceNumberSpinBox->setMaximum(extent[3]);
	ui.sagittalSliceNumberSlider->setMinimum(extent[2]);
	ui.sagittalSliceNumberSlider->setMaximum(extent[3]);
}
void MedicalViewer::initLookupTable()
{
	pixMin=0;
	pixMax=0;
	val1 = 0;
	val2 = 0;
	//xmin = 0;
	//xmax = 255;
	// On fixe les SpinBox et NumberSlider pour minPix
	ui.MinPixNumberSpinBox->setMinimum(xmin);
	ui.MinPixNumberSpinBox->setMaximum(xmax);
	ui.MinPixNumberSlider->setMinimum(xmin);
	ui.MinPixNumberSlider->setMaximum(xmax);

	// On fixe les SpinBox et NumberSlider pour maxPix
	ui.MaxPixNumberSpinBox->setMinimum(xmin);
	ui.MaxPixNumberSpinBox->setMaximum(xmax);
	ui.MaxPixNumberSlider->setMinimum(xmin);
	ui.MaxPixNumberSlider->setMaximum(xmax);
	// On fixe les SpinBox et NumberSlider pour val1
	ui.val1NumberSpinBox->setMinimum(0);
	ui.val1NumberSpinBox->setMaximum(100);
	ui.val1NumberSlider->setMinimum(0);
	ui.val1NumberSlider->setMaximum(100);

	// On fixe les SpinBox et NumberSlider pour val2
	ui.val2NumberSpinBox->setMinimum(0);
	ui.val2NumberSpinBox->setMaximum(100);
	ui.val2NumberSlider->setMinimum(0);
	ui.val2NumberSlider->setMaximum(100);

	// L'affichage d'une image ne se fait qu'avec des niveaux de gris ou des
	//   couleurs indexés sur une table de 0 à 255 (lookup table)

	// Création d'une lookup table
	sliceLookUpTable = vtkSmartPointer<vtkLookupTable>::New();

	// A customiser (cf question ?? du TP)!!
	sliceLookUpTable->SetRange(0, xmax);

	sliceLookUpTable->SetValueRange(0.0, 1.0); // from black to white
	sliceLookUpTable->SetSaturationRange(val1,val2); // color saturation
	sliceLookUpTable->SetRampToLinear();
	sliceLookUpTable->Build();

}

void MedicalViewer::initSliceTools()
{

	// slice axial
	displayAxial = false;
	axialActor = vtkSmartPointer<vtkImageActor>::New();
	axialColor = vtkSmartPointer<vtkImageMapToColors>::New();

	// slice coronal
	displayCoronal=false;
	coronalActor = vtkSmartPointer<vtkImageActor>::New();
	coronalColor = vtkSmartPointer<vtkImageMapToColors>::New();

	// slice sagittal
	displaySagittal=false;
	sagittalActor = vtkSmartPointer<vtkImageActor>::New();
	sagittalColor = vtkSmartPointer<vtkImageMapToColors>::New();
}

void MedicalViewer::initRayCast(){
	ui.R1_SpinBox->setMinimum(0);
	ui.R1_SpinBox->setMaximum(100);

	ui.G1_SpinBox->setMinimum(0);
	ui.G1_SpinBox->setMaximum(100);

	ui.B1_SpinBox->setMinimum(0);
	ui.B1_SpinBox->setMaximum(100);

	ui.R2_SpinBox->setMinimum(0);
	ui.R2_SpinBox->setMaximum(100);

	ui.G2_SpinBox->setMinimum(0);
	ui.G2_SpinBox->setMaximum(100);

	ui.B2_SpinBox->setMinimum(0);
	ui.B2_SpinBox->setMaximum(100);

	ui.R3_SpinBox->setMinimum(0);
	ui.R3_SpinBox->setMaximum(100);

	ui.G3_SpinBox->setMinimum(0);
	ui.G3_SpinBox->setMaximum(100);

	ui.B3_SpinBox->setMinimum(0);
	ui.B3_SpinBox->setMaximum(100);

	ui.R4_SpinBox->setMinimum(0);
	ui.R4_SpinBox->setMaximum(100);

	ui.G4_SpinBox->setMinimum(0);
	ui.G4_SpinBox->setMaximum(100);

	ui.B4_SpinBox->setMinimum(0);
	ui.B4_SpinBox->setMaximum(100);

	ui.Op1_SpinBox->setMinimum(0);
	ui.Op1_SpinBox->setMaximum(1000);

	ui.Op2_SpinBox->setMinimum(0);
	ui.Op2_SpinBox->setMaximum(1000);

	ui.Op3_SpinBox->setMinimum(0);
	ui.Op3_SpinBox->setMaximum(1000);

	ui.Op4_SpinBox->setMinimum(0);
	ui.Op4_SpinBox->setMaximum(1000);

	for(int i=0;i<12;i++)
		RGBVals[i]=0;

	for(int i=0;i<4;i++)
		opacityVals[i]=0;

	opacity = vtkPiecewiseFunction::New();
	color = vtkColorTransferFunction::New();
	volumeProperty = vtkVolumeProperty::New();
	compositeFunction = vtkVolumeRayCastCompositeFunction::New();
	volumeMapper = vtkVolumeRayCastMapper::New();
	volume = vtkVolume::New();

	volumeProperty->SetColor(color);
	volumeProperty->SetScalarOpacity(opacity);
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();

	vtkImageCast* imageCast=vtkImageCast::New();
	imageCast->SetInput(reader->GetOutput());
	imageCast->SetOutputScalarTypeToUnsignedShort();

	volumeMapper->SetInput(imageCast->GetOutput());
	volumeMapper->SetVolumeRayCastFunction(compositeFunction);

	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	dispRayon=false;
	isDisplayed=false;
}


void MedicalViewer::displayAxialSlice(int sliceNumber)
{

	/* ************ Choisir des couleurs appropriées pour l'image ****************/
	// Mapper l'image à travers la lookup table
	axialColor->SetLookupTable(sliceLookUpTable);
	axialColor->SetInput(volumeImage);


	/* ************ Calculer la partie 2D du volume 3D à afficher ****************/
	axialActor->SetInput(axialColor->GetOutput());
	axialActor->SetDisplayExtent(
	                             extent[0], extent[1],
	                             extent[2], extent[3],
	                             sliceNumber, sliceNumber);

	/* ************ Afficher l'image dans le renderer Vtk ************************/
	// On ajoute ce nouvel acteur à la scène (s'il n'existe pas déjà).
	if (! (ren->GetActors())->IsItemPresent(axialActor))
		ren->AddActor(axialActor);

	ren->ResetCamera();

	// On raffraîchit l'affichage
	vtkWidget->update();
}

void MedicalViewer::displayHisto()
{
	int ignoreZero = 0;



	int numComponents = reader->GetOutput()->GetNumberOfScalarComponents();

	if( numComponents > 3 )
	{
		std::cout << "Error: cannot process an image with "
		          << numComponents << " components!" << std::endl;
		exit(-1);
	}


	// Create a vtkXYPlotActor
	vtkSmartPointer<vtkXYPlotActor> plot =
		vtkSmartPointer<vtkXYPlotActor>::New();
	plot->ExchangeAxesOff();
	plot->SetLabelFormat( "%g" );
	plot->SetXTitle( "Level" );
	plot->SetYTitle( "Frequency" );
	plot->SetXValuesToValue();




	double xmax = 0.;
	double ymax = 0.;

	double colors[3][3] = {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 } };

	const char* labels[3] = {
		"Red", "Green", "Blue" };

	// Process the image, extracting and plotting a histogram for each
	// component
	for( int i = 0; i < numComponents; ++i )
	{
		vtkSmartPointer<vtkImageExtractComponents> extract =
			vtkSmartPointer<vtkImageExtractComponents>::New();
		extract->SetInputConnection( reader->GetOutputPort() );
		extract->SetComponents( i );
		extract->Update();

		double range[2];
		extract->GetOutput()->GetScalarRange( range );

		vtkSmartPointer<vtkImageAccumulate> histogram =
			vtkSmartPointer<vtkImageAccumulate>::New();
		histogram->SetInputConnection( extract->GetOutputPort() );
		histogram->SetComponentExtent(
		                              0,
		                              static_cast<int>(range[1])-static_cast<int>(range[0])-1,0,0,0,0 );
		histogram->SetComponentOrigin( range[0],0,0 );
		histogram->SetComponentSpacing( 1,0,0 );
		histogram->SetIgnoreZero( ignoreZero );
		histogram->Update();

		if( range[1] > xmax )
		{
			xmax = range[1];
		}
		if( histogram->GetOutput()->GetScalarRange()[1] > ymax )
		{
			ymax = histogram->GetOutput()->GetScalarRange()[1];
		}

		plot->AddInput( histogram->GetOutput() );

		if( numComponents > 1 )
		{

			plot->SetPlotColor(i,colors[i]);
			plot->SetPlotLabel(i,labels[i]);
			plot->LegendOn();

		}
	}

	this->xmin = 0;
	this->xmax = xmax;



	plot->SetXRange( 0,xmax);
	plot->SetYRange( 0, ymax );
	plot->SetHeight(1.0);
	plot->SetWidth(1.0);
	plot->SetPosition(0,0);


	// Visualize the histogram(s)
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renHisto->AddActor(plot);
	ui.HistoLayout->update();
}

void MedicalViewer::displayCoronalSlice(int sliceNumber)
{

	/* ************ Choisir des couleurs appropriées pour l'image ****************/
	// Mapper l'image à travers la lookup table
	coronalColor->SetLookupTable(sliceLookUpTable);
	coronalColor->SetInput(volumeImage);


	/* ************ Calculer la partie 2D du volume 3D à afficher ****************/
	coronalActor->SetInput(coronalColor->GetOutput());
	coronalActor->SetDisplayExtent(
	                               extent[0], extent[1],
	                               sliceNumber, sliceNumber,
	                               extent[4], extent[5]);

	/* ************ Afficher l'image dans le renderer Vtk ************************/
	// On ajoute ce nouvel acteur à la scène (s'il n'existe pas déjà).
	if (! (ren->GetActors())->IsItemPresent(coronalActor))
		ren->AddActor(coronalActor);

	ren->ResetCamera();

	// On raffraîchit l'affichage
	vtkWidget->update();
}

void MedicalViewer::displaySagittalSlice(int sliceNumber)
{

	/* ************ Choisir des couleurs appropriées pour l'image ****************/
	// Mapper l'image à travers la lookup table
	sagittalColor->SetLookupTable(sliceLookUpTable);
	sagittalColor->SetInput(volumeImage);


	/* ************ Calculer la partie 2D du volume 3D à afficher ****************/
	sagittalActor->SetInput(sagittalColor->GetOutput());
	sagittalActor->SetDisplayExtent(
	                                sliceNumber, sliceNumber,
	                                extent[2], extent[3],
	                                extent[4], extent[5]);

	/* ************ Afficher l'image dans le renderer Vtk ************************/
	// On ajoute ce nouvel acteur à la scène (s'il n'existe pas déjà).
	if (! (ren->GetActors())->IsItemPresent(sagittalActor))
		ren->AddActor(sagittalActor);

	ren->ResetCamera();

	// On raffraîchit l'affichage
	vtkWidget->update();
}


/*  Slots **********************************************************************************/
void MedicalViewer::slotIsAxialDisplaied(bool display)
{
	displayAxial = display;
	if (displayAxial) {

		displayAxialSlice(ui.axialSliceNumberSpinBox->value());

		// Place la caméra correctement pour voir le slice
		ren->ResetCamera();
	}

	else
		ren->RemoveActor(axialActor);

	vtkWidget->update();
}

void MedicalViewer::slotIsCoronalDisplaied(bool display)
{
	displayCoronal = display;
	if (displayCoronal) {

		displayCoronalSlice(ui.coronalSliceNumberSpinBox->value());

		// Place la caméra correctement pour voir le slice
		ren->ResetCamera();
	}

	else
		ren->RemoveActor(coronalActor);

	vtkWidget->update();
}

void MedicalViewer::slotIsSagittalDisplaied(bool display)
{
	displaySagittal = display;
	if (displaySagittal) {

		displaySagittalSlice(ui.sagittalSliceNumberSpinBox->value());

		// Place la caméra correctement pour voir le slice
		ren->ResetCamera();
	}

	else
		ren->RemoveActor(sagittalActor);

	vtkWidget->update();
}

void MedicalViewer::slotAxialSliceNumberChanged(int sliceNumber)
{
	if (displayAxial)
		displayAxialSlice(sliceNumber);


}

void MedicalViewer::slotCoronalSliceNumberChanged(int sliceNumber)
{
	if (displayCoronal)
		displayCoronalSlice(sliceNumber);

}

void MedicalViewer::slotSagittalSliceNumberChanged(int sliceNumber)
{
	if (displaySagittal)
		displaySagittalSlice(sliceNumber);

}



void MedicalViewer::slotMinPixNumberChanged(int min)
{
	pixMin=min;
	if (min<pixMax){
		sliceLookUpTable->SetRange(pixMin,pixMax);
		ui.HistoLayout->update();
		vtkWidget->update();
	}

}

void MedicalViewer::slotMaxPixNumberChanged(int max)
{
	pixMax=max;
	if (max>pixMin){
		sliceLookUpTable->SetRange(pixMin,pixMax);
		// ui.HistoLayout->update();
		vtkWidget->update();

	}
}


void MedicalViewer::slotVal1NumberChanged(int val)
{
	val1=val;
	sliceLookUpTable->SetSaturationRange(val1/100.0,val2/100.0);
	ui.HistoLayout->update();
	vtkWidget->update();
}

void MedicalViewer::slotVal2NumberChanged(int val)
{
	val2=val;
	sliceLookUpTable->SetSaturationRange(val1/100.0,val2/100.0);
	ui.HistoLayout->update();
	vtkWidget->update();
}

void MedicalViewer::initIsoSurface()
{
	ui.isoSpinBox->setMinimum(0);
	ui.isoSpinBox->setMaximum(xmax);

	surfExtractor = vtkContourFilter::New();
	surfExtractor->SetInputConnection( reader->GetOutputPort());
	surfExtractor->SetValue(0, isoVal);

	surfNormals = vtkPolyDataNormals::New();
	surfNormals->SetInputConnection(surfExtractor->GetOutputPort());
	surfNormals->SetFeatureAngle(60.0);

	surfStripper = vtkStripper::New();
	surfStripper->SetInputConnection(surfNormals->GetOutputPort());

	vtkPolyDataMapper *surfMapper = vtkPolyDataMapper::New();
	surfMapper->SetInputConnection(surfStripper->GetOutputPort());
	surfMapper->ScalarVisibilityOff();

	surfActor = vtkActor::New();
	surfActor->SetMapper(surfMapper);
	surfActor->GetProperty()->SetDiffuseColor(1, .49, .25);
	surfActor->GetProperty()->SetSpecular(.3);
	surfActor->GetProperty()->SetSpecularPower(20);
}

void MedicalViewer::updateIsoSurface()
{
	if(isoVal<xmax){
		surfExtractor->SetValue(0, isoVal);
	}
}

void MedicalViewer::updateRayCast(){
	int i=0;
	while(i<12 && RGBVals[i]>=0 && RGBVals[i]<=100)
		i++;

	if(i==12){
		double intervalle=xmax/3.0;

		opacity->RemoveAllPoints();
		opacity->AddSegment(0, opacityVals[0], intervalle,opacityVals[1]);
		opacity->AddSegment(intervalle,opacityVals[1],2*intervalle,opacityVals[2]);
		opacity->AddSegment(2*intervalle,opacityVals[2],3*intervalle,opacityVals[3]);


		color->RemoveAllPoints();
		color->AddRGBPoint(0, RGBVals[0]/100.0, RGBVals[1]/100.0, RGBVals[2]/100.0);
		color->AddRGBPoint(1*intervalle, RGBVals[3]/100.0, RGBVals[4]/100.0, RGBVals[5]/100.0);
		color->AddRGBPoint(2*intervalle, RGBVals[6]/100.0, RGBVals[7]/100.0, RGBVals[8]/100.0);
		color->AddRGBPoint(3*intervalle, RGBVals[9]/100.0, RGBVals[10]/100.0, RGBVals[11]/100.0);
	}
}

void MedicalViewer::slotIsIsoSurfDisplaied(bool display){
	displayIsoSurf = display;
	if (displayIsoSurf) {
		displayIsoSurface(ui.isoSpinBox->value());
		updateIsoSurface();
		// Place la caméra correctement pour voir le slice
		ren->ResetCamera();
	}

	else{
		ren->RemoveActor(surfActor);
	}

	vtkWidget->update();
}

void MedicalViewer::slotIsRayonDisplaied(bool disp){
	dispRayon = disp;
	if (dispRayon) {
		displayRayon();
		updateRayCast();
		// Place la caméra correctement pour voir le slice
		ren->ResetCamera();
	}

	else{
		ren->RemoveVolume(volume);
		isDisplayed=false;
	}

	vtkWidget->update();
}


void MedicalViewer::displayIsoSurface(int isoVal){
	/* ************ Afficher l'image dans le renderer Vtk ************************/
	// On ajoute ce nouvel acteur à la scène (s'il n'existe pas déjà).
	this->isoVal=isoVal;
	if (! (ren->GetActors())->IsItemPresent(surfActor)){
		ren->AddActor(surfActor);
	}
	else{
		updateIsoSurface();
	}

	ren->ResetCamera();

	// On raffraîchit l'affichage
	vtkWidget->update();
}

void MedicalViewer::displayRayon(){
	if (!isDisplayed){
		ren->AddVolume(volume);
		isDisplayed=true;
	}
	else{
		updateRayCast();
	}

	ren->ResetCamera();

	// On raffraîchit l'affichage
	vtkWidget->update();
}

void MedicalViewer::slotIsoSurfNumberChanged(int isoVal){
	this->isoVal=isoVal;
	if (displayIsoSurf){
		//ren->RemoveActor(surfActor);
		updateIsoSurface();
		/*	if (! (ren->GetActors())->IsItemPresent(surfActor))
			ren->AddActor(surfActor);

			ren->ResetCamera();

			// On raffraîchit l'affichage */
		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotR1NumberChanged(int r1){
	RGBVals[0]=r1;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotG1NumberChanged(int g1){
	RGBVals[1]=g1;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotB1NumberChanged(int b1){
	RGBVals[2]=b1;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotR2NumberChanged(int r2){
	RGBVals[3]=r2;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotG2NumberChanged(int g2){
	RGBVals[4]=g2;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotB2NumberChanged(int b2){
	RGBVals[5]=b2;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotR3NumberChanged(int r3){
	RGBVals[6]=r3;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotG3NumberChanged(int g3){
	RGBVals[7]=g3;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotB3NumberChanged(int b3){
	RGBVals[8]=b3;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotR4NumberChanged(int r4){
	RGBVals[9]=r4;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotG4NumberChanged(int g4){
	RGBVals[10]=g4;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotB4NumberChanged(int b4){
	RGBVals[11]=b4;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}


void MedicalViewer::slotOp1NumberChanged(int op1){
	opacityVals[0]=op1;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotOp2NumberChanged(int op2){
	opacityVals[1]=op2;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotOp3NumberChanged(int op3){
	opacityVals[2]=op3;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

void MedicalViewer::slotOp4NumberChanged(int op4){
	opacityVals[3]=op4;
	if(dispRayon){
		updateRayCast();

		ren->ResetCamera();
		vtkWidget->update();
	}
}

