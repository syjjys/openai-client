#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QLabel>
#include "curl_utils.h"
#include <thread>
#include <QThread>
#include <QObject>
#include <QInputDialog>
#include <QLineEdit>
#include <fstream>
#include <QScrollBar>
#include <QTimer>

using namespace std;

string url = "http://137.175.94.62:8689/api/openai/chat/response";
string token = "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";

void write_key_to_local(string key)
{
    std::ofstream keyFile("key_store.txt", std::ios_base::app);
    if (keyFile.is_open()) {
        keyFile << key << "," << std::endl;
        keyFile.close();
    } else {
        std::cerr << "Failed to open file!" << std::endl;
    }
}

enum class ChatType {
    User,
    Gpt
};

void write_chat_to_local(string text, ChatType ct)
{
    std::ofstream chatFile("chat_store.txt", std::ios_base::app);
    if (chatFile.is_open()) {
        if (ct == ChatType::User) {
            chatFile << "__User__" << text << std::endl;
        } else if (ct == ChatType::Gpt) {
            chatFile << "__Gpt__" << text << std::endl;
        }
        chatFile.close();
    } else {
        std::cerr << "Failed to open file!" << std::endl;
    }
}
class UserWidget : public QWidget
{
public:
    UserWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        // 添加需要显示的组件
        QLabel *userLabel = new QLabel("USER: ", this);
        QLabel *gptLabel = new QLabel("GPT: ", this);
        userText = new QTextEdit(this);
        gptText = new QTextEdit(this);
        userLabel->setGeometry(7, 10, userLabel->width(), userLabel->height());
        gptLabel->setGeometry(10, 70, gptLabel->width(), gptLabel->height());
        userText->setGeometry(70, 10, 680, 50);
        gptText->setGeometry(70, 70, 680, 150);
        this->setGeometry(10, 10, 300, 200);
        this->setFixedSize(740, 300);
    }
    void setUserText(QString ut);
    void setGptText(QString gt);
private:
    QTextEdit *userText;
    QTextEdit *gptText;
};

void UserWidget::setUserText(QString ut) {
    this->userText->setText(ut);
    write_chat_to_local(ut.toStdString(), ChatType::User);
}

void UserWidget::setGptText(QString gt) {
    this->gptText->setText(gt);
    write_chat_to_local(gt.toStdString(), ChatType::Gpt);
}

string get_Api_key() {
    std::ifstream inFile("key_store.txt");
    if (inFile.is_open()) {
        std::string line;
        string key;
        while (std::getline(inFile, line, ',')) {
            line.data();
            if (!line.empty()) {
                key = line;
                key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
            }
        }
        inFile.close();
        return key;
    } else {
        std::cerr << "Failed to open file!" << std::endl;
    }
    return "";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    std::ofstream keyfile("key_store.txt", std::ios_base::out | std::ios_base::app);
    std::ofstream chatfile("chat_store.txt", std::ios_base::out | std::ios_base::app);

    if (keyfile.is_open()) {
        keyfile.close();
    }

    if (chatfile.is_open()) {
        chatfile.close();
    }

    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/winicon.ico"));

    string tok = get_Api_key();
    if(!tok.empty()) {
        token = tok;
    }

    connect(ui->actionModify_Key, &QAction::triggered, this, [=]() {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Input Api Key"),
                                             tr("Key Field: "), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()) {
            token = text.toStdString();
            write_key_to_local(token);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

UserWidget* uw;

void MainWindow::show_gpt_response(QString qstr) {
    uw->setGptText(qstr);
}

int i = 0;
string question = "";
void MyThread::run() {
    std::string response = get_response_from_ai_us(url, token, question);
    emit updateUI(QString::fromStdString(response));
}
int maxBottom = 0;

void MainWindow::on_submit_clicked()
{
    QString requestText = ui->userEdit_2->toPlainText();
    ui->userEdit_2->clear();
    QScrollArea *scrollArea = ui->scrollArea_4;
    UserWidget* userwidget = new UserWidget;
    if (i == 0) {
    scrollArea->setWidgetResizable(true);
    // 将控件添加到scrollarea中
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(userwidget);
    widget->setLayout(layout);
    scrollArea->setWidget(widget);
    userwidget->setUserText(requestText);
    i++;
    } else  {
        QLayout *qlayout = scrollArea->widget()->layout();
        qlayout->addWidget(userwidget);
        userwidget->setUserText(requestText);
    }

    uw = userwidget;
    question = requestText.toStdString();
    MyThread *thread2 = new MyThread;
    connect(thread2, &MyThread::updateUI, this, &MainWindow::show_gpt_response);
    thread2->start();
    QTimer::singleShot(10, this, [this](){
        ui->scrollArea_4->verticalScrollBar()->setValue(ui->scrollArea_4->verticalScrollBar()->maximum());
    });
}

