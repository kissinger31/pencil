#include "stroketool.h"

#include "scribblearea.h"
#include "strokemanager.h"
#include "editor.h"

#ifdef Q_OS_MAC
extern "C" {
    void detectElCapitan();
    void disableCoalescing();
    void enableCoalescing();
}
#else
extern "C" {
    void detectElCapitan() {}
    void disableCoalescing() {}
    void enableCoalescing() {}
}
#endif

StrokeTool::StrokeTool( QObject *parent ) :
BaseTool( parent )
{
    detectElCapitan();
}

void StrokeTool::startStroke()
{
    mFirstDraw = true;
    mLastPixel = getCurrentPixel();
    
    mStrokePoints.clear();

    //Experimental
    QPointF startStrokes =  m_pStrokeManager->interpolateStart(mLastPixel);
    mStrokePoints << mEditor->view()->mapScreenToCanvas( startStrokes );

    mStrokePressures.clear();
    mStrokePressures << m_pStrokeManager->getPressure();

    disableCoalescing();
}

bool StrokeTool::keyPressEvent(QKeyEvent *event)
{
    switch ( event->key() ) {
    case Qt::Key_Alt:
        mScribbleArea->setTemporaryTool( EYEDROPPER );
        return true;
    case Qt::Key_Space:
        mScribbleArea->setTemporaryTool( HAND ); // just call "setTemporaryTool()" to activate temporarily any tool
        return true;
    }
    return false;
}

bool StrokeTool::keyReleaseEvent(QKeyEvent *event)
{
    return true;
}

void StrokeTool::endStroke()
{
    m_pStrokeManager->interpolateEnd();
    mStrokePressures << m_pStrokeManager->getPressure();
    mStrokePoints.clear();
    mStrokePressures.clear();

    enableCoalescing();
}

void StrokeTool::drawStroke()
{
    QPointF pixel = getCurrentPixel();
    if ( pixel != mLastPixel || !mFirstDraw )
    {

        // get last pixel before interpolation initializes
        QPointF startStrokes =  m_pStrokeManager->interpolateStart(getLastPixel());
        mStrokePoints << mEditor->view()->mapScreenToCanvas( startStrokes );
        mStrokePressures << m_pStrokeManager->getPressure();

    }
    else
    {
        mFirstDraw = false;
    }
}
