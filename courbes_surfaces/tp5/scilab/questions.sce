exec('util.sce')
exec('schemas.sce')
exec('tp5mat.sce')

function traceChaikin()
  P=inputpoly()
  
  P1=chaikinIter(P,1)
  plot(P1(1,:),P1(2,:))
  P2=chaikinIter(P,2)
  plot(P2(1,:),P2(2,:))
  P3=chaikinIter(P,3)
  plot(P3(1,:),P3(2,:))
  xs2jpg('q11chaikin.jpg')
endfunction

function traceClark(rang)
  P=inputpoly()
  
  P1=catmullClarkIter(P,1)
  plot(P1(1,:),P1(2,:))
  P2=catmullClarkIter(P,2)
  plot(P2(1,:),P2(2,:))
  P3=catmullClarkIter(P,3)
  plot(P3(1,:),P3(2,:))
  xs2jpg('q11clark.jpg')
endfunction

function traceFourPts(rang)
  P=inputpoly()
  
  P1=fourPtsIter(P,1)
  plot(P1(1,:),P1(2,:))
  P2=fourPtsIter(P,2)
  plot(P2(1,:),P2(2,:))
  P3=fourPtsIter(P,3)
  plot(P3(1,:),P3(2,:))
  xs2jpg('q11fourPts.jpg')
endfunction