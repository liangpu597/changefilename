#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QDebug>
#include<QtWidgets/QMessageBox>
#include <QByteArray>
#include<QStandardItemModel>


//JSON相关头文件
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

//浏览器
#include <QDesktopServices>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);

    //删除按钮
    QPushButton *button_delete = new QPushButton(this);
    button_delete->move(130,0);
    button_delete->setObjectName("button_delete");
    button_delete->setCheckable(true);
    button_delete->setText("删除");

    //序号按钮
    QPushButton *button_index = new QPushButton(this);
    button_index->move(200,0);
    button_index->setObjectName("button_index");
    button_index->setCheckable(true);
    button_index->setText("序号");

    //插入按钮
    QPushButton *button_insert = new QPushButton(this);
    button_insert->move(270,0);
    button_insert->setObjectName("button_insert");
    button_insert->setCheckable(true);
    button_insert->setText("插入");

    //扩展名按钮
    QPushButton *button_append = new QPushButton(this);
    button_append->move(340,0);
    button_append->setObjectName("button_insert");
    button_append->setCheckable(true);
    button_append->setText("扩展名");

    //关于按钮
    QPushButton *button_about = new QPushButton(this);
    button_about->move(410,0);
    button_about->setObjectName("button_insert");
    button_about->setCheckable(true);
    button_about->setText("关于");

    //添加这些按钮到buttongroup
    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(button_delete);
    buttonGroup->addButton(button_index);
    buttonGroup->addButton(button_insert);
    buttonGroup->addButton(button_append);
    buttonGroup->addButton(button_about);

    buttonGroup->setExclusive(true);

    //一打开界面默认选中的是删除按钮 不然的话，后面必须点击一次才可以
    button_delete->setChecked(true);
    //默认页面在第1页
    ui->stackedWidget->setCurrentIndex(0);

    //buttongroup中的按钮一旦被选中，对应的页面也要跟着变化,只要一变化，就调用槽函数on_changeButtonGroup
    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(on_changeButtonGroup(int)));

    //buttonGroup->setParent(this);
    //button1->setParent(this);

    //widget_1 = new QListWidget(ui->stackedWidget);
    //widget_1->addItem("widget_1");
    //widget_1->addItem("123");
    //widget_2 = new QListWidget(ui->stackedWidget);
    //widget_2->addItem("widget_2");

    //ui->stackedWidget->addWidget(widget_1);
    //ui->stackedWidget->addWidget(widget_2);
    /*
    ui->label_3->setPixmap(QPixmap(":/google.jpg"));
    ui->label_3->setScaledContents(true);
    */

    //设置列宽均分窗口
    //ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    //设置tablewidget
    ui->tableWidget->setColumnCount(3);  //表格为3列

    //设置表头
    QStringList header;
    header<<tr("原来的文件名")<<tr("新的文件名")<<tr("状态");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    //表头加粗
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    //为单元格添加图标 文字
    //ui->tableWidget->insertRow(0);
    //ui->tableWidget->setItem(0,0, new QTableWidgetItem(QIcon(":/google.jpg"), "测试123"));
    //ui->tableWidget->setItem(0,0,new QTableWidgetItem("Jan"));

    /*
     * 插入一个下拉菜单
    QComboBox *comBox = new QComboBox();
    comBox->addItem("Y");
    comBox->addItem("N");
    ui->tableWidget->insertRow(1);
    ui->tableWidget->setCellWidget(1,1,comBox);
    */
  //QPixmap myPix(":/new/prefix1/1.png");

  //ui->label_3->show();

    //只要点击了不同的按钮后，发出信号后，都会调用同一个槽函数click_Changed
    connect(ui->radioButton_LastModified, SIGNAL(clicked()), this, SLOT(click_Changed()));
    connect(ui->radioButton_FileName,SIGNAL(clicked()),this, SLOT(click_Changed()));
    connect(ui->radioButton_FileSize,SIGNAL(clicked()), this, SLOT(click_Changed()));
    connect(ui->radioButton_UP,SIGNAL(clicked()), this, SLOT(click_Changed()));
    connect(ui->radioButton_Down,SIGNAL(clicked()), this, SLOT(click_Changed()));


    //设置字体颜色
    //ui->tableWidget->item(0,1)->setForeground(Qt::red);

    //没有数据行数是0
    //int row_count = ui->tableWidget->rowCount(); //获取表单行数
    /*
    ui->tableWidget->insertRow(row_count); //插入新行
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("123"));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem("123"));
    */
    /*
    ui->tableWidget->insertRow(row_count);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("123"));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem("123"));
    */
    /*
    ui->tableWidget->insertRow(row_count);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("2145"));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem("123"));
    */
    //qDebug()<<ui->tableWidget->rowCount()<<endl;

    //QTableWidgetItem *item = new QTableWidgetItem();


    //进度条隐藏
    ui->progressBar->hide();
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//关联信号和槽

/*
    networkManager= new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResultUpdate(QNetworkReply*)));
    networkManager->get(QNetworkRequest(QUrl("http://qt.nokia.com")));
*/
}

Widget::~Widget()
{
    delete ui;
}

//打开文件夹槽函数
void Widget::on_pushButton_OpenFileDir_clicked()
{

    //默认从当前程序所在路径打开 这里是打开的文件夹
    QString file_path = QFileDialog::getExistingDirectory(this);
    if(file_path.isEmpty())
    {
        return;
    }
    else
    {

       //将得到的文件夹的名字存放在private QString file_dir中
       SetFileDir(file_path);

       //清空tablewidget 不然下一次会接着这个继续
       ui->tableWidget->clearContents();
       ui->tableWidget->setRowCount(0);   //上面那样删除 删除完后 还有行号 这里把行号设置为0

       //得到该文件下面的所有文件
       FindFile2(file_path);

       //qDebug()<<"open file number is: "<<ui->tableWidget->rowCount()<<endl;

    }

}

//遍历文件夹得到文件夹下面的所有文件 按照一定的顺序 并且把结果写在tableWidget
int Widget:: FindFile2(const QString& _filePath)
{
    QDir dir(_filePath);

    if (!dir.exists()) {
        return -1;
    }

  //取到所有的文件和文件名，但是去掉.和..的文件夹（这是QT默认有的）
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    //文件夹优先
    /*
     * 然后发现有点奇怪 按照文件名排名 up的时候就不需要加reversed
     * 但是其他的 时间，文件大小这些是反着的 up的时候加reverserd down的时候不需要添加
     * */

    //然后先要得到按钮信息，进行排序
    if(ui->radioButton_FileName->isChecked())  //按照文件名大小
    {
        if(ui->radioButton_UP->isChecked()) //升序
        {
            //dir.setSorting(QDir::DirsFirst| QDir::Name);
            dir.setSorting(QDir::Name);

        }
        else if(ui->radioButton_Down->isChecked()) //降序
        {
            //dir.setSorting(QDir::DirsFirst| QDir::Name| QDir::Reversed);
            dir.setSorting(QDir::Name| QDir::Reversed);

        }
    }
   else if(ui->radioButton_LastModified->isChecked())  //按照上次修改时间
    {
        if(ui->radioButton_UP->isChecked())  //升序
        {
            //dir.setSorting(QDir::DirsFirst| QDir::Time| QDir::Reversed);
            dir.setSorting(QDir::Time| QDir::Reversed);

        }
        else if(ui->radioButton_Down->isChecked()) //降序
        {
            //dir.setSorting(QDir::DirsFirst| QDir::Time );
            dir.setSorting(QDir::Time );

        }
    }
    else if(ui->radioButton_FileSize->isChecked())  //按照文件尺寸
    {
        if(ui->radioButton_UP->isChecked())  //升序
        {    dir.setSorting(QDir::Size| QDir::Reversed);
             //dir.setSorting(QDir::DirsFirst| QDir::Size| QDir::Reversed);

        }
        else if(ui->radioButton_Down->isChecked())  //降序
        {    dir.setSorting(QDir::Size);
             //dir.setSorting(QDir::DirsFirst| QDir::Size);
        }
    }
    else   //default
    {
        dir.setSorting(QDir::DirsFirst);
    }
    //dir.setSorting(QDir::DirsFirst| QDir::Time);

    //转化成一个list
    QFileInfoList list = dir.entryInfoList();
    QStringList infolist = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);   //列出文件 以及不列出文件系统中的特殊文件. 以及..
    if(list.size()< 1 ) {
        return -1;
    }

    //递归算法的核心部分
    int i=0;
    do{
        QFileInfo fileInfo = list.at(i);   //这里可能包含文件夹 所以这里的索引下标是以文件夹为单位进行的
        //如果是文件夹，递归
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            FindFile2(fileInfo.filePath());
        }
        else{
            for(int m = 0; m <infolist.size(); m++) {

                //qDebug() << infolist.at(m);
                //这里只有文件名 没有路径

                ui->tableWidget->insertRow(m);
                ui->tableWidget->setItem(m,0,new QTableWidgetItem(infolist.at(m)));  //插入的只是文件名，且QTableWidgetItem必须是QString类型

            }
            break;
        }//end else
        i++;
    } while(i < list.size());

    //底部在就绪位置列出:有 ***个文件
    ui->label_3->setText(QString::number(list.size())+"个文件");

    //底部显示文件夹路径地址
    ui->label_20->setTextInteractionFlags(Qt::TextSelectableByMouse);  //设置label可以被复制
    ui->label_20->setText(_filePath);

    return 0;
}


/*
QString Widget::ReplaceOriginalFileName(int currentIndex, int number)
{

    //例子2
    QStringList BigNumber;
            BigNumber
                << QStringLiteral("一")   //0
                << QStringLiteral("二")   //1
                << QStringLiteral("三")   //2
                << QStringLiteral("四")   //3
                << QStringLiteral("五")   //4
                << QStringLiteral("六")   //5
                << QStringLiteral("七")   //6
                << QStringLiteral("八")   //7
                << QStringLiteral("九")   //8
                << QStringLiteral("十")   //9
                << QStringLiteral("百")   //10
                << QStringLiteral("千")   //11
                << QStringLiteral("万")   //12
                << QStringLiteral("零")   //13
                ;


    switch (currentIndex) {
            case 0: return QString::number(number);
            case 1:  //01
                    if(number >=0 & number <10)
                       return  "0"+QString::number(number);
                    else if(number<100 && number >=10)
                        return  QString::number(number);

            case 2:    //001
                    if(number >=0 & number <10)
                        return  "00"+QString::number(number);
                    else if(number >=10 && number <100)
                        return  "0"+QString::number(number);
                    else if(number >=100 && number <1000)
                        return  QString::number(number);

            case 3:  //0001
                    if(number >=0 & number <10)
                        return  "000"+QString::number(number);
                    else if(number >=10 && number <100)
                        return  "00"+QString::number(number);
                    else if(number >=100 && number <1000)
                        return  "0"+QString::number(number);
                    else if(number >=1000 && number <10000)
                        return  QString::number(number);

            case 4:  //00001
                if(number >=0 & number <10)
                    return  "0000"+QString::number(number);
                else if(number >=10 && number <100)
                    return  "000"+QString::number(number);
                else if(number >=100 && number <1000)
                    return  "00"+QString::number(number);
                else if(number >=1000 && number <10000)
                    return  "0"+QString::number(number);
                else if(number >=10000 && number <100000)
                    return  QString::number(number);

            //case 5:  qDebug()<<"一"+QString::number(3)-"一";
            case 5:
                if(number > 0 && number < 10)
                {
                    return BigNumber[number-1];
                }

                else if(number >=10 && number <100)
                {
                    if(number > 10 && number <=19)
                    {
                        return BigNumber[9]+BigNumber[number-11];
                    }
                    else if(number %10 == 0)
                    {
                        if(number ==10) {return  BigNumber[9];}
                        else {return BigNumber[number/10 - 1]+ BigNumber[9];}
                    }
                    else
                    {
                        return BigNumber[number/10 -1]+BigNumber[9]+ BigNumber[number %10 - 1];
                    }
                }
                else if(number >=100 && number <1000)
                {
                    //100 200 300
                    if(number %100 ==0) {return BigNumber[number/100-1]+BigNumber[10];}
                    //110 210 350
                    else if(number %10 ==0 && number%100 !=0) {return BigNumber[number/100-1]+BigNumber[10]+BigNumber[(number/10)%10 - 1]+ BigNumber[9];}
                    else if((number/10)%10 ==0)
                    {  //101 205
                    return BigNumber[number/100-1]+BigNumber[10]+BigNumber[13]+BigNumber[number%10-1];
                    }
                    else
                    {
                        // 325 154
                        return BigNumber[number/100-1]+BigNumber[10]+BigNumber[(number/10)%10 -1]+BigNumber[9]+BigNumber[number%10-1];
                    }
                }

//这里出现了问题 当有100个文件的时候  最后以后文件命名的是九十八 这里不知道是为什么
    }

}*/


/*
QString Widget::IndexInsert(int currentIndex, int number, QString OriginalFileName, int flag)
{
    int index = ui->spinBox_Index->value();
    if(flag ==0)          //当从前往后的时候 位数大于字符串长度的时候 允许其有空格
    {
        QString PrefixFormat =  ReplaceOriginalFileName(currentIndex, number);
        QString temp = OriginalFileName;
        temp.insert(index, PrefixFormat);
        return temp;
    }
    if(flag == 1)          //当从后往前的时候  如果移动的位数比字符串长度还大 这个时候应该不允许
    {
        if(index > OriginalFileName.length())
        {
             QMessageBox::critical(NULL, "警告", "移动的位数超过了文件名: "+OriginalFileName+" 的长度,该操作无法正常进行 修改失败 保持原来的文件名不变");
             return OriginalFileName;
        }
        else
        {
            QString PrefixFormat =  ReplaceOriginalFileName(currentIndex, number);
            QString temp = OriginalFileName;
            temp.insert(OriginalFileName.length() - index, PrefixFormat);
            return temp;
        }

    }


}*/

//存储文件夹路径
void Widget::SetFileDir(QString file)
{
    file_dir = file;
    //qDebug()<<"文件夹的路径是："<<file<<endl;

}

//得到文件夹路径
QString Widget::GetFileDir()
{
    return file_dir;

}



//文件排序按钮被选中后的槽函数
void Widget::click_Changed()
{
    QString fileDir = GetFileDir();   //获取存储的文件夹的路径
    QDir dir(fileDir);

    if (!dir.exists()) {
        return ;
    }
    //同上，去掉.和..的文件夹
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    //判断是哪个按钮被选中的
    if(ui->radioButton_FileName->isChecked() && ui->radioButton_UP->isChecked())
    { dir.setSorting(QDir::Name);}
    else if(ui->radioButton_FileName->isChecked() && ui->radioButton_Down->isChecked())
     {dir.setSorting(QDir::Name| QDir::Reversed);}
    else if(ui->radioButton_LastModified->isChecked()&&ui->radioButton_UP->isChecked())
    {dir.setSorting(QDir::Time| QDir::Reversed);}
    else if(ui->radioButton_LastModified->isChecked() && ui->radioButton_Down->isChecked())
    {dir.setSorting(QDir::Time );}
    else if(ui->radioButton_FileSize->isChecked() && ui->radioButton_UP->isChecked() )
    { dir.setSorting(QDir::Size| QDir::Reversed);}
    else if(ui->radioButton_FileSize->isChecked() && ui->radioButton_Down->isChecked())
    {dir.setSorting(QDir::Size);}

    //参见网上的 不用递归 也能查找到当前文件夹下面的所有文件
    for(int i=0; i<dir.count(); ++i)
    {
        //这里的插入前面没有清零，前面打开文件夹操作的时候已经调用了FindFile2函数进行了排序操作然后插入了。这是在原来已经有的基础上再次进行的插入，好像也没有问题一样
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(dir[i]));
    }
    ui->tableWidget->setRowCount(dir.count());     //这里需要设置新的行数 不然的话 会接着原来的一起来 这样行数每次都会增加到原来的一倍


}


//预览按钮槽函数
void Widget::on_pushButton_Display_1_clicked()
{

    //tablewidget里面是空的，必须要打开一个文件夹才行
    if(ui->tableWidget->rowCount() == 0)
    {
         QMessageBox::critical(NULL, "警告", "没有打开任何文件和文件夹");
         return;
    }

    //文本框有内容
    switch (buttonGroup->checkedId()) {  //得到buttonGroup中选择的是哪一个组别
        case -2: //删除
            deleteString();  //删除函数
            break;
        case -3:  //序号
            appednIndex();     //插入序号函数
            break;

        case -4:  //任意位置插入任何字符函数    
            insertString();
            break;

        case -5:  //改变扩展名
            changeappend();
            break;
    }
    /*
    //先获取到使用的是哪一个前缀

    int currentIndex;
    int counts = ui->tableWidget->rowCount();
    switch (ui->comboBox_Prefix->currentIndex()) {
       case 0: currentIndex = 0;
                break;
    case 1:
        if(counts>100)
        {
            QMessageBox::critical(NULL, "警告", "该范围只适用与0-99个文件");
            return;
        }
        currentIndex = 1;
             break;
    case 2:
        if(counts>1000)
        {
            QMessageBox::critical(NULL, "警告", "该范围只适用与0-999个文件");
            return;
        }
        currentIndex = 2;
             break;
    case 3:
        if(counts>10000)
        {
            QMessageBox::critical(NULL, "警告", "该范围只适用与0-9999个文件");
            return;
        }

        currentIndex = 3;
             break;
    case 4:
        if(counts>100000)
        {
            QMessageBox::critical(NULL, "警告", "该范围只适用与0-99999个文件");
            return;
        }

        currentIndex = 4;
             break;
    case 5:
        currentIndex = 5;
             break;
    default:
        qDebug()<<"什么也没有发现"<<endl;
        break;

    }

    //记录没有改名成功的
    int countRightChanged =0;
    QString ChoosedPrefix="";

    for (int i = 0; i < counts; ++i)
    {

        //去掉原文件后缀 .以及点后面的全部删除 只是保留文件名
        QString fileNameRemoveSuffix = ui->tableWidget->item(i,0)->text();
        int index;
        index = fileNameRemoveSuffix.lastIndexOf(".");  //一般来说，扩展名为最后一个“.”之后的字符；
        fileNameRemoveSuffix.truncate(index);//把它cate掉

        ChoosedPrefix = ReturnNewFileName(currentIndex, i+1, fileNameRemoveSuffix);
        if(ChoosedPrefix =="") {return;}

        QString OlderFileDir = GetFileDir()+"/"+ui->tableWidget->item(i,0)->text();


        //得到新的文件名
        QString NewFileName ="";
        if(ui->radioButton_Change_Suffix->isChecked())
        {
            NewFileName = ChoosedPrefix+"."+ ui->comboBox_End->currentText();
        //QString NewFileName = ui->comboBox_Prefix->currentText()+"."+ ui->comboBox_End->currentText();
        //qDebug()<<"新的文件名是: "<<NewFileName<<endl;
        }
        else
        {
            QFileInfo fileinfo;
            fileinfo = QFileInfo(OlderFileDir);
            QString file_suffix = fileinfo.suffix();
            NewFileName = ChoosedPrefix+"."+ file_suffix;
        }

        //向tableWidget中动态添加行 第一个参数表示行号 第二个参数表示列号 第三个为要显示的数据
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(NewFileName));
        //设置字体颜色 更改成功的设置为红色 更改失败的暂时设置为黑色

        //if( ui->tableWidget->item(i,0)->text() ==  ui->tableWidget->item(i,1)->text())
            //ui->tableWidget->item(i,1)->setForeground(QBrush(QColor(0,255,0)));
        //else
        //{
            //ui->tableWidget->item(i,1)->setForeground(QBrush(QColor(255,0,0)));
        //}
        //
        //更改成功(255,0,0)   更改失败： 1.只更改了后缀(0,0,255) 2.没有更改后缀(0,255,255)
        if(fileNameRemoveSuffix == ChoosedPrefix)    //除了后缀前后都是一样的
        {
            if(ui->radioButton_Change_Suffix->isChecked())    //只是更改成功了后缀 前面的文件名没有更改成功
                ui->tableWidget->item(i,1)->setForeground(QBrush(QColor(0,0,255)));
            else if(ui->radioButton_Keep_Original_Suffix->isChecked())    //后缀也没有改 保持的原来的样子
                ui->tableWidget->item(i,1)->setForeground(QBrush(QColor(0,255,255)));

        }
        else
        {
            ui->tableWidget->item(i,1)->setForeground(QBrush(QColor(255,0,0)));
        }

    }
    */
}

//buttongroup槽函数
void Widget::on_changeButtonGroup(int)
{
    QPushButton *checkedButton = qobject_cast<QPushButton*>(buttonGroup->checkedButton());
    QString checkedBtnObjectName = checkedButton->objectName();
    //qDebug()<<tr("被选中按钮的objectName==%1").arg(checkedBtnObjectName);
    //qDebug()<<buttonGroup->checkedId();    //-2删除 -3 -4

    int buttonid = buttonGroup->checkedId();  //在buttongroup中，每一个button都有一个id
    //根据id 设置stackwidget的显示
    switch (buttonid) {
    case -2:  //删除
        ui->stackedWidget->setCurrentIndex(0);
        break;

    case -3:  //序号
         ui->stackedWidget->setCurrentIndex(1);
        break;

    case -4: //插入
        ui->stackedWidget->setCurrentIndex(2);
        break;
    case -5: //扩展名
        ui->stackedWidget->setCurrentIndex(3);
        break;
    case -6: //关于
        ui->stackedWidget->setCurrentIndex(4);
        break;
    }
}

//删除函数
void Widget::deleteString()
{
   int counts = ui->tableWidget->rowCount(); //得到tablewidget有多少行
   for (int i = 0; i < counts; ++i) //遍历每一行
   {

       //去掉原文件后缀 .以及点后面的全部删除 只是保留文件名
       QString fileNameRemoveSuffix = ui->tableWidget->item(i,0)->text();
       int index;
       index = fileNameRemoveSuffix.lastIndexOf(".");  //一般来说，扩展名为最后一个“.”之后的字符；
       fileNameRemoveSuffix.truncate(index);//把它cate掉

       //开始删除,会改变原来字符的长度 这里用一个temp
       QString stringtemp = fileNameRemoveSuffix;

       //1.删除文件名中的***   (这个目前还没有做)



       int begin = ui->spinBox->value();      //从第*个字符开始
       int index_2 = ui->spinBox_2->value();   //删除*个
       //2.从前往后删除
       if(ui->radioButton_Delete_From_Begin->isChecked())
       {  stringtemp.remove(begin-1,index_2);  } //删除，原字符会改变 函数是从0开始的 所以-1}

       //3.从后往前删除,这个就比较复杂了
       else if(ui->radioButton_Delete_From_End->isChecked())
       {
           //解决办法：(1)判断legth(stringtemp) 与 begin的大小
           if(stringtemp.length() > begin)   //大，直接与原来的一样
           {
               if(stringtemp.length() - begin - index_2 >= 0)
               {
                   stringtemp.remove(stringtemp.length()-begin- index_2+1, index_2);
               }
               else
               {
                   stringtemp.remove(0,stringtemp.length()-begin+1);
               }
           }
           else if(stringtemp.length() ==  begin)    //相等则为空
           {
               stringtemp.remove(0,1);
           }
           else if(stringtemp.length() < begin)
           {
               begin;
           }

       }
       //删除多了，最后可能文件名为空的情况
       //QString a="123";
       if(stringtemp=="")
       {
            QMessageBox::critical(NULL, "警告", "文件名不能为空");
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(""));
            continue;
       }


       ui->tableWidget->setItem(i,1,new QTableWidgetItem(stringtemp+"."+getFileAppend(i)));
       /*这里好像有问题 长度不对 特别是里面有中文的情况下
       int lengthstring = stringtemp.length();
       qDebug()<<lengthstring<<endl;
       */

       //stringtemp.chop(3); 删除末尾3个字符


   }

/*
   //这种方法改变了原来的结果
   QString data = "hello world!";
   data.chop(15);    //data = hello wor
   qDebug()<<data.length()<<endl;

*/
}


//任意位置插入字符函数
void Widget::insertString()
{
    int counts = ui->tableWidget->rowCount();
    QString stringtemp="";
    for(int i = 0; i < counts; ++i)
        {
            stringtemp = getFileName(i);    //返回tablewidget第i行的文件名，除去后缀
            //必须先任意位置插入 头部尾部插入顺序任意
            stringtemp.insert(ui->spinBox_5->value(), ui->lineEdit_6->text());  //在第*个字符后插入*
            stringtemp.insert(0, ui->lineEdit_4->text()); //头
            stringtemp.append(ui->lineEdit_5->text());  //尾

            ui->tableWidget->setItem(i,1,new QTableWidgetItem(stringtemp+"."+getFileAppend(i)));
        }


}

//返回tablewidget第i行的文件名，除去.后缀
QString Widget::getFileName(int i)
{
    QString fileNameRemoveSuffix = ui->tableWidget->item(i,0)->text();
    int index;
    index = fileNameRemoveSuffix.lastIndexOf(".");  //一般来说，扩展名为最后一个“.”之后的字符；
    fileNameRemoveSuffix.truncate(index);//把它cate掉
    return fileNameRemoveSuffix;
}

//返回tablewidget第i行的文件名后缀 不含.
QString  Widget::getFileAppend(int i)
{
    QString fileNameRemoveSuffix = ui->tableWidget->item(i,0)->text();
    int index;
    index = fileNameRemoveSuffix.lastIndexOf(".");  //一般来说，扩展名为最后一个“.”之后的字符；

    QString nameTxt=fileNameRemoveSuffix.right(fileNameRemoveSuffix.length()-index-1);
    return nameTxt;

}

//更改扩展名函数
void Widget::changeappend()
{

   int counts = ui->tableWidget->rowCount();
   for (int i = 0; i < counts; ++i)
   {
        QString original_append =getFileAppend(i);  //原来的后缀
        QString filename = getFileName(i);         //原来的前缀
        QString now_append = ui->lineEdit_2->text();  //新的输入的后缀名

        if(ui->radioButton_3->isChecked())   //后缀大写
        {
            if(now_append!="")
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"."+now_append.toUpper()));
            else   //没有输入新的 则把原来的后缀改成大写
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"."+original_append.toUpper()));
        }
        if(ui->radioButton_4->isChecked())    //后缀全部小写
        {
            if(now_append != "")
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"."+now_append.toLower()));
            else  //同上
             ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"."+original_append.toLower()));
        }
        if(ui->radioButton_5->isChecked())    //删除扩展名 注意：这里一旦选择 调用槽函数 把输入的扩展名那里置为空
        {
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename));
        }

    }


}

//删除扩展名按钮的槽函数(将输入的扩展名那个框置为空)
void Widget::on_radioButton_5_clicked()
{
    ui->lineEdit_2->setText("");
}



//执行修改槽函数
void Widget::on_pushButton_ChangeFileName_1_clicked()
{
    int counts = ui->tableWidget->rowCount();
    if(counts == 0)
    {
         QMessageBox::critical(NULL, "警告", "没有打开任何文件");
         return;
    }

    int countRightChanged =0;   //修改成功的个数

    for (int i = 0; i < counts; ++i)
    {
        QString NewFileName =ui->tableWidget->item(i,1)->text();  //新的文件名 包含后缀

        //注：为空的情况是在删除那里删多了导致为空

        if(NewFileName !="")   //不为空
        {
            //1.不合逻辑 在序号插入那里标红的 不重新命名
            if(ui->tableWidget->item(i,1)->foreground() == QColor(255,0,0))
            {
                ui->tableWidget->setItem(i,2,new QTableWidgetItem("改名失败 原因：超出范围"));   //状态栏显示红
                ui->tableWidget->item(i,2)->setForeground(QColor(255,0,0));
            }
            else //2.正常的
            {
                ui->tableWidget->setItem(i,2,new QTableWidgetItem("成功"));   //状态栏显示绿
                ui->tableWidget->item(i,2)->setForeground(QColor(0,255,0));
            }

            //更名
            QString OlderFileDir = GetFileDir()+"/"+ui->tableWidget->item(i,0)->text();  //老的文件名全路径
            QString NewFileNameDir = GetFileDir()+"/"+ NewFileName;
            QFile file1(OlderFileDir);
            //bool ok = file1.rename(NewFileNameDir);
            file1.rename(NewFileNameDir);
            countRightChanged++;

            //进度条显示，设置数值
            ui->progressBar->show();
            ui->progressBar->setValue((i+1)*100/counts);

        }

        //QString NewFileNameDir = GetFileDir()+"/"+ NewFileName;
        //QFile file1(OlderFileDir);
        //bool ok = file1.rename(NewFileNameDir);
        //if(ui->tableWidget->item(i,0)->text() != ui->tableWidget->item(i,1)->text())
        //{countRightChanged++;}

    }

    //显示成功个数 失败个数
    QString messboxInformation = "该目录下一共有："+QString::number(counts)+" 个文件, 成功: "+QString::number( countRightChanged)+" 个, 失败: "+ QString::number(counts-countRightChanged)+" 个" ;
    QMessageBox::about(NULL, "About", messboxInformation);


    //这里添加一个功能：在点击确定后将更改后的文件名显示在原来的文件名中


    /*最后还要清空列表 不然下一次会接着这个继续  这个函数删除所有条目内容，单元格全为 NULL，清除选中状态，但是表头的设置内容都保留，表头文本等照旧显示，表格行数、列数也不变。
    *这里还有一个函数 clear() 单元格全为 NULL，清除选中状态，水平表头和垂直表头的设置内容清空，但是会保留表格的行数、列数，表头清掉 之后，会使用默认的数字行号、列号。
    */
    //ui->tableWidget->clearContents();
    //ui->tableWidget->setRowCount(0);   //上面那样删除 删除完后 还有行号 这里把行号设置为0

}

//添加序号函数
void Widget::appednIndex()
{
    int counts = ui->tableWidget->rowCount();  //所有行数,也就是需要更改的文件名的个数
    switch (ui->comboBox->currentIndex()) {  //下拉菜单选择

        case 0:  //1~9
            if(counts>9)
            {
                QMessageBox::critical(NULL, "警告", "该范围只适用编号为1-9的文件");
                return;
            }
            singleIndex();  //添加序号: 1. 1~9函数
            break;
        case 1:  //01~99
            if(counts>99)
            {
                QMessageBox::critical(NULL, "警告", "该范围只适用编号为01-99的文件");
                return;
            }
            doubleIndex();  //添加序号: 2. 01~99函数
            break;
        case 2:
            if(counts>999)
            {
                QMessageBox::critical(NULL, "警告", "该范围只适用编号为001-999的文件");
                return;
            }
            tripleIndex();  //添加序号: 3. 001~999函数
            break;
        case 3:
            if(counts>9999)
            {
                QMessageBox::critical(NULL, "警告", "该范围只适用编号为0001-9999的文件");
                return;
            }
            fourIndex();  //添加序号： 4.0001~9999函数
            break;
        case 4:
            if(counts>99999)
            {
                QMessageBox::critical(NULL, "警告", "该范围只适用与00001-99999个文件");
                return;
            }
            fiveIndex();  //添加序号： 5. 00001~99999函数
            break;
        case 5:
            limitless();   //添加函数： 6： 1~~无限
             break;
        case 6:  //一 ~ 这个目前还没有做
            break;
    default:
        break;

    }

}

//添加序号: 1. 1~9函数
void Widget::singleIndex()
{
    int begin = ui->spinBox_3->value();   //得到开始序号
    int step = ui->spinBox_4->value();    //得到步长

    int counts = ui->tableWidget->rowCount();  //总行数
    for(int i=0; i< counts; ++i)
    {
        QString filename = getFileName(i);  //得到第i行的文件名(不含前缀)
        QString append = getFileAppend(i);  //得到文件第i行文件名后缀,不含.
        int number = begin + i*step;   //当前序号
        QString numberString = QString::number(number, 10);   //10进制数转换成QString
        switch (ui->comboBox_2->currentIndex()) { //序号下拉菜单
            case 0: //1.序号<文件名>
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                else   //总个数<9 因为初始值和步长的原因，当前值>9 标红 保持原有 不重新命名
                    {

                          ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                          ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));  //该框字体设置为红
                    }
                break;
            case 1:  //2.<文件名>序号
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                else  //原因同上
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                    ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));
                }
                break;
            case 2: //功能待增

            break;
        }
    }

}

//添加序号: 2. 01~99函数
void Widget::doubleIndex()
{
    int begin = ui->spinBox_3->value();   //得到开始序号
    int step = ui->spinBox_4->value();    //得到步长

    int counts = ui->tableWidget->rowCount();
    for(int i=0; i< counts; ++i)
    {
        QString filename = getFileName(i);
        QString append = getFileAppend(i);
        int number = begin + i * step;
        QString numberString = QString::number(number, 10);  //转换成10进制
        switch (ui->comboBox_2->currentIndex()) {
            case 0: //序号<文件名>
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("0"+numberString+filename+"."+append));
                else if(number >9 && number <= 99)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                }
                else   //原因同上 标红 重命名失败
                    {

                          ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                          ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));
                    }
                break;
            case 1:  //<文件名>序号
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"0"+numberString+"."+append));
                else if(number > 9 &&number <=99)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                else
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                    ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));
                }
                break;
            case 2: //功能待定
            break;
        }

        //ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
    }

}
//添加序号: 3. 001~999函数
void Widget::tripleIndex()
{
    int begin = ui->spinBox_3->value();   //得到开始序号
    int step = ui->spinBox_4->value();    //得到步长

    int counts = ui->tableWidget->rowCount();
    for(int i=0; i< counts; ++i)
    {
        QString filename = getFileName(i);
        QString append = getFileAppend(i);
        int number = begin + i * step;
        QString numberString = QString::number(number, 10);  //转换成10进制
        switch (ui->comboBox_2->currentIndex()) {
            case 0: //序号<文件名>
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("00"+numberString+filename+"."+append));
                else if(number >9 && number <= 99)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("0"+numberString+filename+"."+append));
                }
                else if(number >99 && number <= 999)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                }
                else   //同上，标红
                    {

                          ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                          ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));
                    }
                break;
            case 1:  //<文件名>序号
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"00"+numberString+"."+append));
                else if(number > 9 &&number <=99)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"0"+numberString+"."+append));
                else if(number >99 && number <= 999)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                else
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                    ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));
                }
                break;
            case 2: //功能待定
            break;
        }

        //ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
    }
}

//添加序号: 4. 0001~9999函数
void Widget::fourIndex()
{
    int begin = ui->spinBox_3->value();   //得到开始序号
    int step = ui->spinBox_4->value();    //得到步长

    ui->comboBox_2->currentIndex();
    int counts = ui->tableWidget->rowCount();

    for(int i=0; i< counts; ++i)
    {
        QString filename = getFileName(i);
        QString append = getFileAppend(i);
        int number = begin + i * step;
        QString numberString = QString::number(number, 10);  //转换成10进制
        switch (ui->comboBox_2->currentIndex()) {
            case 0: //序号<文件名>
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("000"+numberString+filename+"."+append));
                else if(number >9 && number <= 99)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("00"+numberString+filename+"."+append));
                }
                else if(number >99 && number <= 999)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("0"+numberString+filename+"."+append));
                }
                else if(number >999 && number <= 9999)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                }

                else   //加了增量后 会出现不和逻辑的，但是没有错，这里直接标红算了
                    {

                          ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                          ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));
                    }
                break;
            case 1:  //<文件名>序号
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"000"+numberString+"."+append));
                else if(number > 9 &&number <=99)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"00"+numberString+"."+append));
                else if(number >99 && number <= 999)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"0"+numberString+"."+append));
                else if(number >999 && number <= 9999)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                else  //标红
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                    ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));
                }
                break;
            case 2:  //功能待定
            break;
        }

    }
}

//添加序号: 5. 00001~99999函数
void Widget::fiveIndex()
{
    int begin = ui->spinBox_3->value();   //得到开始序号
    int step = ui->spinBox_4->value();    //得到步长

    ui->comboBox_2->currentIndex();
    int counts = ui->tableWidget->rowCount();

    for(int i=0; i< counts; ++i)
    {
        QString filename = getFileName(i);
        QString append = getFileAppend(i);
        int number = begin + i * step;
        QString numberString = QString::number(number, 10);  //转换成10进制
        switch (ui->comboBox_2->currentIndex()) {
            case 0: //序号<文件名>
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("0000"+numberString+filename+"."+append));
                else if(number >9 && number <= 99)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("000"+numberString+filename+"."+append));
                }
                else if(number >99 && number <= 999)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("00"+numberString+filename+"."+append));
                }
                else if(number >999 && number <= 9999)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem("0"+numberString+filename+"."+append));
                }
                else if(number >9999 && number <= 99999)
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                }

                else   //加了增量后 会出现不和逻辑的，但是没有错，这里直接标红算了
                    {

                          ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                          ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));
                    }
                break;
            case 1:  //<文件名>序号
                if(number <=9)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"0000"+numberString+"."+append));
                else if(number > 9 &&number <=99)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"000"+numberString+"."+append));
                else if(number >99 && number <= 999)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"00"+numberString+"."+append));
                else if(number >999 && number <= 9999)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+"0"+numberString+"."+append));
                else if(number >9999 && number <= 99999)
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                else
                {
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                    ui->tableWidget->item(i,1)->setForeground(QColor(255,0,0));
                }
                break;
            case 2:
            break;
        }

    }
}

//添加序号: 6. 1~无限
void Widget::limitless()
{
    int begin = ui->spinBox_3->value();   //得到开始序号
    int step = ui->spinBox_4->value();    //得到步长

    ui->comboBox_2->currentIndex();
    int counts = ui->tableWidget->rowCount();

    for(int i=0; i< counts; ++i)
    {
        QString filename = getFileName(i);
        QString append = getFileAppend(i);
        int number = begin + i * step;
        QString numberString = QString::number(number, 10);  //转换成10进制
        switch (ui->comboBox_2->currentIndex()) {
            case 0: //序号<文件名>
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(numberString+filename+"."+append));
                    break;
            case 1:  //<文件名>序号
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(filename+numberString+"."+append));
                    break;
            case 2:
                break;
        }
    }
}

//刷新按钮槽函数 将更改后的新文件名显示在item(i,0)上
void Widget::on_pushButton_clicked()
{
    //1.先将当前的列表置为空
    ui->tableWidget->setRowCount(0);   //上面那样删除 删除完后 还有行号 这里把行号设置为0
    //2.得到当前文件夹路径
    QString nowFileDirName = GetFileDir();
    FindFile2(nowFileDirName);

    //进度条
    ui->progressBar->setValue(0);  //置为0
    ui->progressBar->hide();   //隐藏

}
//检查更新槽函数
void Widget::on_pushButton_2_clicked()
{


    QUrl url("https://api.github.com/repos/liangpu597/changefilename/releases/latest");
    QNetworkRequest request;
    request.setUrl(url);
/*
    m_currentReply = networkManager->get(request);  // GET
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResultUpdate(QNetworkReply*)));
    QEventLoop eventLoop;
    connect(networkManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
*/
    m_currentReply = networkManager->get(request);  // GET
}

void Widget::replyFinished(QNetworkReply *reply)
{
    QString str =(QString) reply->readAll();//读取接收到的数据
    QJsonDocument d = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject sett2 = d.object();
    double newrelease= sett2.value("tag_name").toString().toDouble();

   //QJsonValue value = sett2.value(QString("tag_name"));
   //qDebug() << "新的版本号是"<<sett2.value("tag_name").toString().toDouble()<<"测试"<<endl;
   //QString(QJsonValue(value).toJson());
   //qDebug() <<"版本号是: "<<VERSION<<endl;

       if(newrelease > VERSION){   //新版本号大于当前版本号
           QMessageBox::StandardButton button;
           button = QMessageBox::question(this, tr("检查更新"),
                   QString(tr("当前版本号%1,最新版本号是%2,是否下载新的版本？").arg(VERSION).arg(newrelease)),
                   QMessageBox::Yes | QMessageBox::No);

               if (button == QMessageBox::Yes){
                   QDesktopServices::openUrl(QUrl("https://github.com/liangpu597/changefilename/releases/"));

           }
       }
       else{
           QMessageBox::information(0, "更新检查","此版本已经是最新发布版本", QMessageBox::Yes);
       }


}
