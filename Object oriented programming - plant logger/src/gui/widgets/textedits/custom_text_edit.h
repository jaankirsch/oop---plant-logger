#pragma once

#include <QTextEdit>

namespace IAS0410::GUI {
class CustomTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    CustomTextEdit(QWidget* parent = nullptr);

private:
    void setupStyle();
};

}
