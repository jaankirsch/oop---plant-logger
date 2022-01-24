#pragma once

#include <QPushButton>

namespace IAS0410::GUI {
class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    CustomButton(QWidget* parent = nullptr);

private:
    void setupStyle();
};

}
