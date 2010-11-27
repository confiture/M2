exec('util.sce')
exec('schemas.sce')
exec('tp5mat.sce')
////////////////////////////////////////////////////////////////////////
function traceChaikin()
  P=inputpoly()
  disp("yeye")
  P1=chaikinIter(P,1)
  plot(P1(1,:),P1(2,:),'r')
  P2=chaikinIter(P,2)
  plot(P2(1,:),P2(2,:),'g')
  P3=chaikinIter(P,3)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q11chaikin.jpg')
endfunction

////////////////////////////////////////////////////////////////////////
function traceClark(rang)
  P=inputpoly()
  
  P1=catmullClarkIter(P,1)
  plot(P1(1,:),P1(2,:),'r')
  P2=catmullClarkIter(P,2)
  plot(P2(1,:),P2(2,:),'g')
  P3=catmullClarkIter(P,3)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q11clark.jpg')
endfunction

////////////////////////////////////////////////////////////////////////
function traceFourPts(rang)
  P=inputpoly()
  
  P1=fourPtsIter(P,1)
  plot(P1(1,:),P1(2,:),'r')
  P2=fourPtsIter(P,2)
  plot(P2(1,:),P2(2,:),'g')
  P3=fourPtsIter(P,3)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q11fourPts.jpg')
endfunction

function traceCorner09025(rang)
  P=inputpoly()
  
  P1=cornerCutIter(P,0.9,0.25,1)
  plot(P1(1,:),P1(2,:),'r')
  P2=cornerCutIter(P,0.9,0.25,2)
  plot(P2(1,:),P2(2,:),'g')
  P3=cornerCutIter(P,0.9,0.25,3)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q21cornerCut0.9-0.25.jpg')
endfunction

function traceCorner105(rang)
  P=inputpoly()
  
  P1=cornerCutIter(P,1,0.5,1)
  plot(P1(1,:),P1(2,:),'r')
  P2=cornerCutIter(P,1,0.5,2)
  plot(P2(1,:),P2(2,:),'g')
  P3=cornerCutIter(P,1,0.5,3)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q21cornerCut1-0.5jpg')
endfunction

function traceCorner11025(rang)
  P=inputpoly()
  
  P1=cornerCutIter(P,1.1,0.25,1)
  plot(P1(1,:),P1(2,:),'r')
  P2=cornerCutIter(P,1.1,0.25,2)
  plot(P2(1,:),P2(2,:),'g')
  P3=cornerCutIter(P,1.1,0.25,3)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q21cornerCut1.1-0.25jpg')
endfunction


function traceCorner025075(rang)
  P=inputpoly()
  
  P3=cornerCutIter(P,0.25,0.75,3)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q21cornerCut0.25-0.75jpg')
endfunction

////////////////////////////////////////////////////////////////////////
function traceToutCorner()
  P=inputpoly()
  
  P3=cornerCutIter(P,0.9,0.25,10)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q21cornerCut0.9-0.25.jpg')
  P1=[]
  P2=[]
  P3=[]
  
  clf
  a=gca()
  set(a,"data_bounds",[0,0;100,100])
  for i=1:size(P,2)
    plot(P(1,i),P(2,i),"go")
  end
  plot(P(1,:),P(2,:),"g-")
  
  P3=cornerCutIter(P,1,0.5,10)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q21cornerCut1-0.5jpg')
  P1=[]
  P2=[]
  P3=[]
  
  clf
  a=gca()
  set(a,"data_bounds",[0,0;100,100])
  for i=1:size(P,2)
    plot(P(1,i),P(2,i),"go")
  end
  plot(P(1,:),P(2,:),"g-")
  
  P3=cornerCutIter(P,1.1,0.25,10)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q21cornerCut1.1-0.25jpg')
  P1=[]
  P2=[]
  P3=[]
    
  clf
  a=gca()
  set(a,"data_bounds",[0,0;100,100])
  for i=1:size(P,2)
    plot(P(1,i),P(2,i),"go")
  end
  plot(P(1,:),P(2,:),"g-")
  
  P3=cornerCutIter(P,0.25,0.75,10)
  plot(P3(1,:),P3(2,:),'b')
  xs2jpg(0,'q21cornerCut0.25-0.75jpg')
endfunction


function traceChou()
  P=inputpoly()
  
  P7=chouFleurIter(P,7)
  plot(P7(1,:),P7(2,:))
  xs2jpg(0,'q21chouFleur7.jpg')
  
  clf
  a=gca()
  set(a,"data_bounds",[0,0;100,100])
  for i=1:size(P,2)
    plot(P(1,i),P(2,i),"go")
  end
  plot(P(1,:),P(2,:),"g-")
  
  P8=chouFleurIter(P,8)
  plot(P8(1,:),P8(2,:))
  xs2jpg(0,'q21chouFleur8.jpg')
  
  clf
  a=gca()
  set(a,"data_bounds",[0,0;100,100])
  for i=1:size(P,2)
    plot(P(1,i),P(2,i),"go")
  end
  plot(P(1,:),P(2,:),"g-")
  
  P9=chouFleurIter(P,9)
  plot(P9(1,:),P9(2,:))
  xs2jpg(0,'q21chouFleur9.jpg')

  clf
  a=gca()
  set(a,"data_bounds",[0,0;100,100])
  for i=1:size(P,2)
    plot(P(1,i),P(2,i),"go")
  end
  plot(P(1,:),P(2,:),"g-")
  
  P10=chouFleurIter(P,10)
  plot(P10(1,:),P10(2,:))
  xs2jpg(0,'q21chouFleur10.jpg')
endfunction

function trace11()
  P=inputpoly()

  P1=chaikinIter(P,1)
  plot(P1(1,:),P1(2,:),'r')
  P2=chaikinIter(P,2)
  plot(P2(1,:),P2(2,:),'g')
  P3=chaikinIter(P,3)
  plot(P3(1,:),P3(2,:),'b')
  
  xs2jpg(0,'q11chaikin.jpg')
  
  clf
  a=gca()
  set(a,"data_bounds",[0,0;100,100])
  
  
  P1=catmullClarkIter(P,1)
  plot(P1(1,:),P1(2,:),'r')
  P2=catmullClarkIter(P,2)
  plot(P2(1,:),P2(2,:),'g')
  P3=catmullClarkIter(P,3)
  plot(P3(1,:),P3(2,:),'b')
  
  for i=1:size(P,2)
    plot(P(1,i),P(2,i),"go")
  end
  plot(P(1,:),P(2,:),"g-")
  
  xs2jpg(0,'q11clark.jpg')
  
  clf
  a=gca()
  set(a,"data_bounds",[0,0;100,100])
  
  P1=fourPtsIter(P,1)
  plot(P1(1,:),P1(2,:),'r')
  P2=fourPtsIter(P,2)
  plot(P2(1,:),P2(2,:),'g')
  P3=fourPtsIter(P,3)
  plot(P3(1,:),P3(2,:),'b')
  
  
  for i=1:size(P,2)
    plot(P(1,i),P(2,i),"go")
  end
  plot(P(1,:),P(2,:),"g-")
  
  xs2jpg(0,'q11fourPts.jpg')
endfunction
