#include "messageDialog.h"

namespace  control{
MessageDialog::MessageDialog(const QString &title, const QString &description,
                       int buttons, QWidget *parent)
    : QDialog(parent),
      m_isClosed(true)
{
    init();
//    setWindowFlags(Qt::FramelessWindowHint
//                   | Qt::Tool
//                   | Qt::WindowStaysOnTopHint);

//    activateWindow();
//    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);

    m_titleWidget->setTitle(title);
    m_description->setText(description);
    setButton(buttons);
    m_warning->setVisible(false);
    m_detail->setVisible(false);
    setFixedSize(400,180);
}

void MessageDialog::init()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0,0,0,0);

    m_titleWidget = new TitleWidget(this);
    connect(m_titleWidget,&TitleWidget::closeCurrentWidget,this,&MessageDialog::close);
    m_titleWidget->setBackgroundColor(QColor("#404249"));
    m_titleWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_mainLayout->addWidget(m_titleWidget);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(24,16,24,16);
    layout->setSpacing(12);
    m_mainLayout->addLayout(layout);

    m_description = new QLabel(this);
    m_description->setWordWrap(true);
    m_description->setAlignment(Qt::AlignHCenter);
    layout->addWidget(m_description);

    m_detail = new QLabel(this);
    m_detail->setWordWrap(true);
    m_detail->setStyleSheet("background-color:rgb(44,44,44);");
    layout->addWidget(m_detail);

    m_edit = new QLineEdit(this);
    m_edit->setMaximumHeight(40);
    m_edit->setObjectName("m_edit");
    layout->addWidget(m_edit);

    m_warning = new QLabel(this);
    m_warning->setMinimumHeight(21);
    m_warning->setObjectName("m_warning");
    m_warning->setAlignment(Qt::AlignVCenter);
    QFont fontWarning( this->font());
    fontWarning.setPixelSize(fontWarning.pixelSize() - 2);
    m_warning->setFont(fontWarning);

    layout->addWidget(m_warning);

    m_pushLayout = new QHBoxLayout();
    m_leftButton = new QPushButton(this);
    m_leftButton->setMinimumHeight(32);
    QFont fontButton( this->font());
    fontButton.setPixelSize(fontButton.pixelSize() + 2);
    m_leftButton->setFont(fontButton);
    m_leftButton->setObjectName("m_leftButton");
    m_leftButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_leftButton,&QPushButton::clicked,this,&MessageDialog::buttonClicked);

    m_middleButton = new QPushButton(this);
    m_middleButton->setObjectName("m_middleButton");
    m_middleButton->setFont(fontButton);
    m_middleButton->setMinimumHeight(32);
    m_middleButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_middleButton,&QPushButton::clicked,this,&MessageDialog::buttonClicked);

    m_rightButton = new QPushButton(this);
    m_rightButton->setObjectName("m_rightButton");
    m_rightButton->setFont(fontButton);
    m_rightButton->setMinimumHeight(32);
    m_rightButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_rightButton,&QPushButton::clicked,this,&MessageDialog::buttonClicked);

    m_pushLayout->addWidget(m_leftButton);

    m_pushLayout->addWidget(m_middleButton);
    m_pushLayout->addWidget(m_rightButton);
    layout->addLayout(m_pushLayout);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setEditMode(false);
}

void MessageDialog::setButton(int buttons)
{
    switch (buttons) {
    case(MessageDialog::OK):
        m_leftButton->setVisible(false);
        m_middleButton->setVisible(false);
        m_rightButton->setText(tr("OK"));
        setButtonValue(m_rightButton,MessageDialog::OK);
        break;

//    case(MessageDialog::YES | MessageDialog::NO):
//        m_leftButton->setText(tr("No"));
//        setButtonValue(m_leftButton,MessageDialog::NO);
//        m_rightButton->setVisible(false);
//        m_middleButton->setText(tr("Yes"));
//        setButtonValue(m_middleButton,MessageDialog::YES);
//        break;
    case(MessageDialog::CANCEL | MessageDialog::SAVE):
        m_leftButton->setText(tr("Cancel"));
        setButtonValue(m_leftButton,MessageDialog::CANCEL);
        m_rightButton->setText(tr("Save"));
        setButtonValue(m_rightButton,MessageDialog::SAVE);
        m_middleButton->setVisible(false);
        break;
    case(MessageDialog::CANCEL | MessageDialog::SAVE | MessageDialog::DoNotSave):
        m_leftButton->setText(tr("Cancel"));
        setButtonValue(m_leftButton,MessageDialog::CANCEL);
        m_rightButton->setText(tr("Save"));
        setButtonValue(m_rightButton,MessageDialog::SAVE);
        m_middleButton->setText(tr("Don't Save"));
        setButtonValue(m_middleButton,MessageDialog::DoNotSave);
        m_pushLayout->insertStretch(1,1);
        m_leftButton->setFixedWidth(100);
        m_rightButton->setFixedWidth(100);
        m_middleButton->setFixedWidth(100);
        this->setMinimumWidth(480);
        break;
    case(MessageDialog::SAVE | MessageDialog::DoNotSave):
        m_leftButton->setText(tr("Don't Save"));
        setButtonValue(m_leftButton,MessageDialog::DoNotSave);
        m_rightButton->setText(tr("Save"));
        setButtonValue(m_rightButton,MessageDialog::SAVE);
        m_middleButton->setVisible(false);
        break;
    default:
        m_leftButton->setText(tr("No"));
        setButtonValue(m_leftButton,MessageDialog::NO);
        m_middleButton->setVisible(false);
        m_rightButton->setText(tr("Yes"));
        setButtonValue(m_rightButton,MessageDialog::YES);
        break;
    }

    m_leftButton->setFocusPolicy(Qt::NoFocus);
    m_middleButton->setFocusPolicy(Qt::NoFocus);
    m_rightButton->setFocusPolicy(Qt::NoFocus);

}

void MessageDialog::setButtonValue(QPushButton *button, BUTTONFLAG flage)
{
    button->setProperty("value",flage);  
}

void MessageDialog::setBottonText(BUTTONFLAG botton, const QString &string)
{
    switch (botton) {
    case(MessageDialog::YES):
        m_rightButton->setText(string);
        break;
    case(MessageDialog::NO):
        m_leftButton->setText(string);
        break;
    default:
        break;
    }
}

void MessageDialog::setDetailText(const QString &text)
{
    if (m_detail){
        m_detail->clear();
        m_detail->setText(text);
    }
}

void MessageDialog::setDetailVisible(bool show)
{
    if(m_detail){
        m_detail->setVisible(show);
    }
}

void MessageDialog::setEditMode(bool edit)
{
    m_edit->setVisible(edit);
    if(edit) {
        setMinimumSize(400,230);
        m_warning->setVisible(true);
    }
}

void MessageDialog::setValidator(const QValidator *validator)
{
    m_edit->setValidator(validator);
}

void MessageDialog::setWarning(const QString &warning)
{
    m_warning->setVisible(true);
    m_warning->setText(warning);
}

void MessageDialog::setEditText(const QString &string)
{
    m_edit->setText(string);
}

QString MessageDialog::editText() const
{
    return m_edit->text();
}

void MessageDialog::setAutoLevelText(const QString& leftText,  const QString &rightText)
{
    m_leftButton->setText(leftText);
    m_middleButton->setText(rightText);
}

void MessageDialog::setDescriptionText(const QString &text)
{
    m_description->clear();
    m_description->setText(text);
}

void MessageDialog::setAutoClosed(bool ok)
{
     m_isClosed = ok;
}

void MessageDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor("#404249"));
    QPainterPath path;
    path.addRoundedRect(0,0,this->width() , this->height() ,16,16);
    painter.drawPath(path);

    QWidget::paintEvent(event);
}

void MessageDialog::buttonClicked()
{

    QPushButton *button = qobject_cast<QPushButton* > (sender());
    auto flag = button->property("value").toInt();
   // qDebug() <<" debug m_isClosed==" << flag;
    if(m_isClosed) {
        done(flag);
    }
    else {
        emit buttonClick(flag);
    }
}

TimerMessageDialog::TimerMessageDialog(const QString &title, const QString &description, int time, int buttons, QWidget *parent) :
    MessageDialog::MessageDialog(title, description, buttons, parent), m_totalSecs(time), m_description(description)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    QString tempStr = description + " " + QString::number(m_totalSecs) + "s";
    qDebug() << "----" << tempStr;
    setDescriptionText(tempStr);
    connect(m_timer, &QTimer::timeout, this, [=]() mutable{
        if(m_totalSecs >= 0)
        {
            m_totalSecs--;
            QString tempStr = description + " " + QString::number(m_totalSecs) + "s";
            qDebug() << "----" << tempStr;
            setDescriptionText(tempStr);
        }
        else
        {
            m_timer->stop();
        }
    });
    m_timer->start();
}

}
