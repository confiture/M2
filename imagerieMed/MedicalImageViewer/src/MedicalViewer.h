/********************************************************************************************
 *  fichier:               MedicalImageViewer.h                                             *
 *  auteur:                C. Fouard, modifié par                                           *
 *  dernière modification: 2010/02/05                                                       *
 *                                                                                          *
 *  description:                                                                            *
 *    Le MedicalViewer est une fenêtre Qt qui contient                                      *
 *       - une partie (à gauche) qui contient une fenêtre 3D Vtk (QVTKWidget / vtkRenderer) *
 *       - une partie (à droite) qui contient les boutons et les paramètres d'affichage.    *
 *                                                                                          *
 *                                                                                          *
 ********************************************************************************************/

// Qt includes
#include <QMainWindow>
#include <QApplication>

// VTK includes
// To use Smart Pointers
#include <vtkSmartPointer.h>
// Render Window
#include <QVTKWidget.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
// Image Data
#include <vtkImageData.h>
// To display slices
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
// To read volume images
#include <vtkMetaImageReader.h>
#include <qstring.h>

// Include GUI  automatically generated from the file
// MedicalViewer.ui (to be modified via QtDesigner).
#include "ui_MedicalViewer.h"
#include <iostream>


// histogramme

#include <vtkSmartPointer.h>

#include <string>

#include <vtkActor.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkImageExtractComponents.h>
#include <vtkJPEGReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkXYPlotActor.h>

//pour isosurfaces
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume16Reader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkOutlineFilter.h"
#include "vtkCamera.h"
#include "vtkStripper.h"
#include "vtkLookupTable.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkProperty.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkImageData.h"
#include "vtkImageMapToColors.h"
#include "vtkImageActor.h"

// pour le lancé de rayon
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkStructuredPointsReader.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolume.h"
#include "vtkColorTransferFunction.h"
#include "vtkCamera.h"
#include "vtkImageCast.h"


class MedicalViewer : public QMainWindow {
	Q_OBJECT

		protected:
	/*  Attributes *******************************************************************************/

	// Vtk visualization window
	QVTKWidget * vtkWidget;
	// Vtk Renderer
	vtkSmartPointer<vtkRenderer> ren;

	// Vtk visualization histogram
	QVTKWidget * vtkHistoWidget;
	// Vtk Renderer
	vtkSmartPointer<vtkRenderer> renHisto;

	// pixMin & pixMax
	int pixMin;
	int pixMax;

	// xmin et xmax sont les pixels minimum et maximum
	int xmin;
	int xmax;

	// Les deux paramètres de la saturations
	int val1;
	int val2;

	// la valeur de la surface isovaleur
	int isoVal;

	//////////////////////////////////////////
	//Les champs pour le lancé de rayons
	/////////////////////////////////////////

	// les couleurs pour le lance de rayon
	vtkColorTransferFunction *color;
	int RGBVals[12];

	//l'opacité
	vtkPiecewiseFunction *opacity;
	int opacityVals[4];

	vtkVolume *volume;
	vtkVolumeProperty *volumeProperty;
	vtkVolumeRayCastCompositeFunction *compositeFunction;
	vtkVolumeRayCastMapper *volumeMapper;

	///////////////////////////////////
	/////////POUR LES ISOSURFACES
	///////////////////////////////////
	// Pour extraire les isosurfaces
	vtkContourFilter* surfExtractor;

	vtkPolyDataNormals* surfNormals;

	vtkStripper* surfStripper;

	vtkPolyDataMapper* surfMapper;

	vtkActor* surfActor;
	//////////////////////////////////////////////

	/// file name
	QString filename;

	/// Volume image displayed on the left part of the window
	vtkImageData * volumeImage;

	vtkMetaImageReader * reader;

	/// Table containing first and last indices of the image in each direction
	/// 0 & 1 -> x; 2 and 3 -> y; 4 & 5 -> z
	int extent[6];
	/// Space between 2 pixels in mm (0->x, 1->y, 2->z)
	double spacing[3];
	/// Image volume origin
	double origin[3];
	/// Image volume center
	double center[3];

	// Common lookup table
	vtkSmartPointer<vtkLookupTable>      sliceLookUpTable;

	// Tools for axial slicer
	vtkSmartPointer<vtkImageMapToColors> axialColor;
	vtkSmartPointer<vtkImageActor>       axialActor;

	vtkSmartPointer<vtkImageActor>      histoActor;
	// Tools for coronal slicer
	vtkSmartPointer<vtkImageMapToColors> coronalColor;
	vtkSmartPointer<vtkImageActor>       coronalActor;

	// Tools for sagittal slicer
	vtkSmartPointer<vtkImageMapToColors> sagittalColor;
	vtkSmartPointer<vtkImageActor>       sagittalActor;

	// Do we display or not slices
	bool displayAxial;
	bool displayCoronal;
	bool displaySagittal;

	// On affiche ou pas les isosurfaces
	bool displayIsoSurf;
	// On affiche ou pas le lancé de rayon
	bool dispRayon;

	bool isDisplayed;

	/*  Slots **********************************************************************************/
	// Signals triggered by Qt GUI
	public slots:
	// Slots pour l'affichage des 3 coupes
	virtual void slotIsAxialDisplaied(bool);
	virtual void slotAxialSliceNumberChanged(int);
	virtual void slotIsCoronalDisplaied(bool);
	virtual void slotCoronalSliceNumberChanged(int);
	virtual void slotIsSagittalDisplaied(bool);
	virtual void slotSagittalSliceNumberChanged(int);
	// Slots pour le choix d'un niveau de gris min et max
	virtual void slotMaxPixNumberChanged(int max);
	virtual void slotMinPixNumberChanged(int min);
	// Slots pour l'utilisation de différentes colormap
	virtual void slotVal1NumberChanged(int val);
	virtual void slotVal2NumberChanged(int val);
	// Slots pour l'affichage de l'isosurface
	virtual void slotIsIsoSurfDisplaied(bool);
	virtual void slotIsoSurfNumberChanged(int);
	// Slots pour l'affichage du lancé de rayon
	virtual void slotIsRayonDisplaied(bool);
	// Pour le point 1 (R,G,B) et l'opacite
	virtual void slotR1NumberChanged(int);
	virtual void slotG1NumberChanged(int);
	virtual void slotB1NumberChanged(int);
	// Pour le point 2 (R,G,B) et l'opacite

	virtual void slotR2NumberChanged(int);
	virtual void slotG2NumberChanged(int);
	virtual void slotB2NumberChanged(int);

	virtual void slotR3NumberChanged(int);
	virtual void slotG3NumberChanged(int);
	virtual void slotB3NumberChanged(int);

	virtual void slotR4NumberChanged(int);
	virtual void slotG4NumberChanged(int);
	virtual void slotB4NumberChanged(int);

	virtual void slotOp1NumberChanged(int);
	virtual void slotOp2NumberChanged(int);
	virtual void slotOp3NumberChanged(int);
	virtual void slotOp4NumberChanged(int);

 public:
	/*  Constructor / Desctructor **************************************************************/
	MedicalViewer(QString filename, QWidget *parent = NULL);
	~MedicalViewer();

	/*  Input / Output *************************************************************************/
	/** Read the filename given in parameters and stores the result image in volumeImage.
	 * @parameter filename path and filename to the meta image (should have a .mha or .mhd suffix).
	 * @returns true if the operation succeded, false otherwise.
	 */
	bool readVolumeImage(const char * filename);

	/** Sets the image info elements of the graphic interface to correct values once the
	 *  image volue has been read.
	 */
	void setImageInfo();


	/*  Image display ***************************************************************************/
	/** Display axial slice at the number sliceNumber in the vtk renderer.
	 *
	 *  \verbatim
	 *       3D Volume                         2D Slice
	 *       ________  /|\
	 *      /|      /|  |                      _______
	 *     /______ / | _|_slice     ===>      /       /        Displayed in
	 *    |  |____|_/|  |  number            /______ /         the window
	 *    | /     |//
	 *    |/______|/
	 *
	 */
	void displayAxialSlice(int sliceNumber);
	void displayCoronalSlice(int sliceNumber);
	void displaySagittalSlice(int sliceNumber);
	void displayIsoSurface(int isoVal);
	void displayHisto();
	void displayRayon();

 protected:
	// Tool methods

	/// Places the vtk renderer at the right place on the window.
	void initWindow();

	// Fabrique l'histogramme
	void initHisto();

	/// Creates once and for all the bricks of the axial slice display pipeline
	/// The plug-in of these briks will be done in displayAxialSlice
	void initSliceTools();

	void initLookupTable();

	// met à jour l'isosurface (lorsque isoVal change et que la checkbox de l'isosurface
	// est cochée)
	void updateIsoSurface();

	// initialise les champs permettant de fabriquer l'isosurface
	void initIsoSurface();

	// initialise les champs permettant de faire le lancé de rayon
	void initRayCast();

	void updateRayCast();
 private:
	/*  Qt GUI exported frome QtDesigner ********************************************************/
	Ui_MedicalViewer ui;
};
