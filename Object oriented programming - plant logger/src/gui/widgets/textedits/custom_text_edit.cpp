#include "custom_text_edit.h"

namespace IAS0410::GUI {

CustomTextEdit::CustomTextEdit(QWidget* parent) :
    QTextEdit{parent}
{
    setupStyle();
}

void CustomTextEdit::setupStyle()
{
    QString style;


    style.append("QTextEdit{background:#ffe6ff; border: 1px solid #6666ff;background-image:url('C:/Users/jaank/Downloads/plant.jpg');}");
    //style.append("QTextEdit{background:#ffe6ff; border: 1px solid #6116ff;}");

    setStyleSheet(style);
}

}
