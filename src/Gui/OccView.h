#include <QWidget>
#include <QShowEvent>
#include <QPoint>

#include <AIS_InteractiveContext.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <TopoDS_Shape.hxx>
#include <V3d_View.hxx>
#include <Aspect_DisplayConnection.hxx>

#ifdef Q_OS_WIN
#include <WNT_Window.hxx>
#else
#include <Xw_Window.hxx>
#endif

class OccView : public QWidget {
    Q_OBJECT
public:
    OccView(QWidget* parent = nullptr);
    void displayShape(const TopoDS_Shape& shape);

protected:
    // 必须重写 paintEvent 和 resizeEvent
    void paintEvent(QPaintEvent*) override { if (!m_view.IsNull()) m_view->Redraw(); }
    void resizeEvent(QResizeEvent*) override;
    void showEvent(QShowEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    QPaintEngine* paintEngine() const override { return nullptr; } // 禁止 Qt 绘制背景

private:
    void initViewer();

    bool m_initialized = false;
    Handle(Aspect_DisplayConnection) m_displayConnection;
    Handle(V3d_Viewer) m_viewer;
    Handle(V3d_View) m_view;
    Handle(AIS_InteractiveContext) m_context;

    QPoint m_lastPos;
};
