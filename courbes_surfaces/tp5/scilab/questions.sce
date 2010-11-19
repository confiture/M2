exec('util.sce')
exec('schemas.sce')
exec('tp5mat.sce')

function traceChaikin(rang)
  P=inputpoly()
  P=chaikinIter(P,rang)
  plot(P(1,:),P(2,:))
endfunction

function traceClark(rang)
  P=inputpoly()
  P=catmullClarkIter(P,rang)
  plot(P(1,:),P(2,:))
endfunction

function traceFourPts(rang)
  P=inputpoly()
  P=fourPtsIter(P,rang)
  plot(P(1,:),P(2,:))
endfunction