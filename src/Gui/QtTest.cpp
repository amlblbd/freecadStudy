#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Qt6 Test Window");
    window.resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&window);
    
    QLabel *label = new QLabel("Hello Qt6 World!", &window);
    label->setAlignment(Qt::AlignCenter);
    
    layout->addWidget(label);
    
    window.setLayout(layout);
    window.show();

    return app.exec();
}