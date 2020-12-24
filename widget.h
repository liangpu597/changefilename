#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QPushButton>

#include <QButtonGroup>

#include<QListWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //递归查找所有文件 并且显示在tabelwidget
    int FindFile2(const QString& _filePath);

    //QString ReplaceOriginalFileName(int currentIndex, int number);
    //QString IndexInsert(int currentIndex, int number, QString OriginalFileName, int flag);

    //存储打开的文件夹的路径
    void SetFileDir(QString file);

    //返回上面这个路径
    QString GetFileDir();


    //返回第i行的文件名，除去.后缀
    QString getFileName(int i);

    //得到文件第i行文件名后缀,不含.
    QString getFileAppend(int i);

    //删除函数
    void deleteString();   //从前往后删除flag=0 反之index=1

    //任意位置插入函数
    void insertString();
    //更改扩展名函数
    void changeappend();

    //添加序号函数
    void appednIndex();

    //添加序号: 1. 1~9函数
    void singleIndex();

    //添加序号: 2. 01~99函数
    void doubleIndex();

    //添加序号: 3. 001~999函数
    void tripleIndex();

    //添加序号： 4.0001~9999函数
    void fourIndex();

    //添加序号： 5. 00001~99999函数
    void fiveIndex();

    //添加函数： 6： 1~~无限
    void limitless();


private slots:
    void on_pushButton_OpenFileDir_clicked();  //打开文件夹槽函数
    void click_Changed();   //文件排序按钮被选中后的槽函数
    void on_changeButtonGroup(int);   //删除，插入，序号等页面变化的槽函数(用于改变页面)
    void on_radioButton_5_clicked();  //删除扩展名按钮的槽函数(将输入的扩展名那个框置为空)
    void on_pushButton_Display_1_clicked();  //预览按钮槽函数
    void on_pushButton_ChangeFileName_1_clicked();  //执行修改槽函数
    void on_pushButton_clicked();  //刷新按钮槽函数 将更改后的新文件名显示在item(i,0)上

private:
    Ui::Widget *ui;

    QString file_dir;  //存储文件夹路径

    //QListWidget * widget_1;
    //QListWidget * widget_2;

    //按钮组
    QButtonGroup *buttonGroup;
    QPushButton *button_delete;  //删除
    QPushButton *button_index;   //序号
    QPushButton *button_insert;   //插入

    /*待实现
    //QPushButton *button_replace;  //替换
    //QPushButton *button_datetime;  //日期时间
    */

    QPushButton *button_append;    //扩展名
    QPushButton *button_about;     //关于

};
#endif // WIDGET_H
