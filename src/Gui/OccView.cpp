#include "OccView.h"

#include <AIS_Shape.hxx>
#include <Aspect_TypeOfTriedronPosition.hxx>
#include <Quantity_Color.hxx>
#include <Quantity_NameOfColor.hxx>
#include <V3d_Viewer.hxx>
#include <QMouseEvent>
#include <QWheelEvent>

#ifndef Q_OS_WIN
#include <X11/Xlib.h>
#endif

OccView::OccView(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);
}

void OccView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!m_view.IsNull())
    {
        m_view->MustBeResized();
    }
}

void OccView::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (!m_initialized)
    {
        initViewer();
        m_initialized = true;
    }
}

void OccView::initViewer()
{
    m_displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(m_displayConnection);
    m_viewer = new V3d_Viewer(driver);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    m_view = m_viewer->CreateView();

#ifdef Q_OS_WIN
    Handle(WNT_Window) window = new WNT_Window((Aspect_Handle)winId());
#else
    Handle(Xw_Window) window = new Xw_Window(m_displayConnection, (Window)winId());
#endif

    m_view->SetWindow(window);
    if (!window->IsMapped())
    {
        window->Map();
    }

    m_view->SetBackgroundColor(Quantity_NOC_GRAY20);
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.08, V3d_ZBUFFER);

    m_context = new AIS_InteractiveContext(m_viewer);
}

void OccView::displayShape(const TopoDS_Shape& shape)
{
    if (m_context.IsNull())
    {
        return;
    }

    m_context->RemoveAll(true);
    Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
    m_context->Display(aisShape, true);
    m_view->FitAll();
}

void OccView::mousePressEvent(QMouseEvent* event)
{
    if (m_view.IsNull())
    {
        return;
    }

    m_lastPos = event->pos();

    if (event->button() == Qt::LeftButton)
    {
        m_view->StartRotation(m_lastPos.x(), m_lastPos.y());
    }
    event->accept();
}

void OccView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_view.IsNull())
    {
        return;
    }

    const QPoint pos = event->pos();
    const int dx = pos.x() - m_lastPos.x();
    const int dy = pos.y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        m_view->Rotation(pos.x(), pos.y());
    }
    else if (event->buttons() & Qt::MiddleButton)
    {
        m_view->Pan(dx, -dy);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        m_view->Zoom(m_lastPos.x(), m_lastPos.y(), pos.x(), pos.y());
    }

    m_lastPos = pos;
    event->accept();
}

void OccView::mouseReleaseEvent(QMouseEvent* event)
{
    event->accept();
}

void OccView::wheelEvent(QWheelEvent* event)
{
    if (m_view.IsNull())
    {
        return;
    }

    const QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull())
    {
        const int steps = numDegrees.y() / 15;
        const QPoint pos = event->position().toPoint();
        if (steps > 0)
        {
            m_view->Zoom(pos.x(), pos.y(), pos.x() - 50, pos.y() - 50);
        }
        else if (steps < 0)
        {
            m_view->Zoom(pos.x(), pos.y(), pos.x() + 50, pos.y() + 50);
        }
    }
    event->accept();
}
