/**
 * @copyright 2020-2022 Uniontech Technology Co., Ltd.
 *
 * @file createvgwidget.cpp
 *
 * @brief VG创建窗口
 *
 * @date 2022-02-18 11:06
 *
 * Author: yuandandan  <yuandandan@uniontech.com>
 *
 * Maintainer: yuandandan  <yuandandan@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "createvgwidget.h"
#include "common.h"

#include <DLabel>
#include <DFontSizeManager>
#include <DWindowCloseButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

CreateVGWidget::CreateVGWidget(QWidget *parent) : DDBase(parent)
{
    initUi();
    initConnection();
    updateData();
}

void CreateVGWidget::initUi()
{
    setFixedSize(800, 538);

    m_curSeclectData.clear();
    m_curDiskItemWidget = nullptr;
    m_waterLoadingWidget = nullptr;

    DPalette paletteTitle;
    QColor colorTitle("#000000");
    colorTitle.setAlphaF(0.9);
    paletteTitle.setColor(DPalette::ToolTipText, colorTitle);

    DPalette paletteSubTitle;
    QColor colorSubTitle("#000000");
    colorSubTitle.setAlphaF(0.7);
    paletteSubTitle.setColor(DPalette::ToolTipText, colorSubTitle);

    DPalette palette1;
    palette1.setColor(DPalette::Text, QColor("#526A7F"));

    DPalette palette2;
    palette2.setColor(DPalette::TextTitle, QColor("#001A2E"));

    DLabel *titleLabel = new DLabel(tr("Create volume group"));
    QFont fontTitle;
    fontTitle.setWeight(QFont::Medium);
    fontTitle.setFamily("Source Han Sans");
    fontTitle.setPixelSize(14);
    titleLabel->setFont(fontTitle);
    titleLabel->setPalette(paletteTitle);
    titleLabel->setAlignment(Qt::AlignCenter);

    QFont fontSubTitle;
    fontSubTitle.setWeight(QFont::Normal);
    fontSubTitle.setFamily("Source Han Sans");
    fontSubTitle.setPixelSize(14);

    QFont font1;
    font1.setWeight(QFont::Normal);
    font1.setFamily("Source Han Sans");
    font1.setPixelSize(12);

    QFont font2;
    font2.setWeight(QFont::Medium);
    font2.setFamily("Source Han Sans");
    font2.setPixelSize(13);
    // 第一个页面
    DLabel *subTitleLabel = new DLabel(tr("Select disks or partitions to create a volume group and set its capacity"), this);
    subTitleLabel->setFont(fontSubTitle);
    subTitleLabel->setPalette(paletteSubTitle);
    subTitleLabel->setAlignment(Qt::AlignCenter);

    m_seclectedLabel = new DLabel(tr("Capacity selected: %1").arg("0GiB"), this);
    m_seclectedLabel->setFont(font1);
    m_seclectedLabel->setPalette(palette1);
    m_seclectedLabel->setAlignment(Qt::AlignCenter);

    DLabel *describeLabel = new DLabel(tr("The selected disks will be converted to dynamic disks, "
                                      "and you will not be able to start installed operating systems from the disks."), this);
    describeLabel->setFont(font1);
    describeLabel->setPalette(palette1);
    describeLabel->setWordWrap(true);
    describeLabel->setAlignment(Qt::AlignCenter);

    QPalette paletteBackground;
    paletteBackground.setColor(QPalette::Base, QColor(0, 0, 0, 0));
    m_diskFrame = new DFrame(this);
    m_diskFrame->setFixedSize(370, 306);
    m_diskFrame->setPalette(paletteBackground);
    m_diskFrame->setBackgroundRole(QPalette::Base);
    m_diskFrame->setFrameShadow(QFrame::Plain);
    m_diskFrame->setFrameShape(QFrame::NoFrame);
    m_diskFrame->setContentsMargins(0, 0, 0, 0);
    m_diskFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_diskScrollArea = new DScrollArea;
    m_diskScrollArea->setFrameShadow(QFrame::Plain);
    m_diskScrollArea->setFrameShape(QFrame::NoFrame);
    m_diskScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
    m_diskScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖向滚动条

    QVBoxLayout *diskScrollAreaLayout = new QVBoxLayout;
    diskScrollAreaLayout->addWidget(m_diskScrollArea);
    diskScrollAreaLayout->setSpacing(0);
    diskScrollAreaLayout->setContentsMargins(0, 0, 0, 0);
    m_diskFrame->setLayout(diskScrollAreaLayout);

    m_partitionFrame = new DFrame(this);
    m_partitionFrame->setFixedSize(370, 306);
    m_partitionFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_partitionFrame->setPalette(paletteBackground);
    m_partitionFrame->setFrameShadow(QFrame::Plain);
    m_partitionFrame->setFrameShape(QFrame::NoFrame);

    m_partitionScrollArea = new DScrollArea;
    m_partitionScrollArea->setFrameShadow(QFrame::Plain);
    m_partitionScrollArea->setFrameShape(QFrame::NoFrame);
    m_partitionScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
    m_partitionScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖向滚动条

    QVBoxLayout *partitionScrollAreaLayout = new QVBoxLayout;
    partitionScrollAreaLayout->addWidget(m_partitionScrollArea);
    partitionScrollAreaLayout->setSpacing(0);
    partitionScrollAreaLayout->setContentsMargins(0, 0, 0, 0);
    m_partitionFrame->setLayout(partitionScrollAreaLayout);

    QLabel *iconNoPartitionLabel =new QLabel(this);
    QIcon iconNoPartition = Common::getIcon("nopartition");
    iconNoPartitionLabel->setPixmap(iconNoPartition.pixmap(QSize(90, 90)));
    iconNoPartitionLabel->setAlignment(Qt::AlignCenter);
    iconNoPartitionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QLabel *noPartitionLabel = new QLabel(this);
    noPartitionLabel->setText(tr("No partitions available"));
    noPartitionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    noPartitionLabel->setAlignment(Qt::AlignCenter);
    noPartitionLabel->setFont(font1);
    noPartitionLabel->setPalette(palette2);

    QVBoxLayout *noPartitionLayout = new QVBoxLayout;
    noPartitionLayout->addStretch();
    noPartitionLayout->addWidget(iconNoPartitionLabel, 0, Qt::AlignCenter);
    noPartitionLayout->addSpacing(16);
    noPartitionLayout->addWidget(noPartitionLabel, 0, Qt::AlignCenter);
    noPartitionLayout->addStretch();
    noPartitionLayout->setSpacing(0);
    noPartitionLayout->setContentsMargins(0, 0, 0, 0);

    DFrame *noPartitionFrame = new DFrame(this);
//    noPartitionFrame->setFixedSize(370, 305);
    noPartitionFrame->setBackgroundRole(DPalette::ItemBackground);
    noPartitionFrame->setLayout(noPartitionLayout);

    QVBoxLayout *noPartitionMainLayout = new QVBoxLayout;
    noPartitionMainLayout->addWidget(noPartitionFrame);
    noPartitionMainLayout->setSpacing(0);
    noPartitionMainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *noPartitionWidget = new QWidget(this);
    noPartitionWidget->setLayout(noPartitionMainLayout);

    m_partitionStackedWidget = new DStackedWidget(this);
    m_partitionStackedWidget->addWidget(m_partitionFrame);
    m_partitionStackedWidget->addWidget(noPartitionWidget);
    m_partitionStackedWidget->setCurrentIndex(0);

    DFrame * line =new DFrame(this);
    paletteBackground.setColor(QPalette::Base, QColor(0, 0, 0, 10));
    line->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setPalette(paletteBackground);
    line->setFixedSize(2, 306);

    QHBoxLayout *selectLayout = new QHBoxLayout(this);
    selectLayout->addWidget(m_diskFrame);
    selectLayout->addSpacing(10);
    selectLayout->addWidget(line);
    selectLayout->addSpacing(10);
    selectLayout->addWidget(m_partitionStackedWidget);
    selectLayout->setSpacing(0);
    selectLayout->setContentsMargins(10, 0, 10, 0);

    QVBoxLayout *firstCenterLayout = new QVBoxLayout;
    firstCenterLayout->addLayout(selectLayout);
    firstCenterLayout->addSpacing(10);
    firstCenterLayout->addWidget(m_seclectedLabel);
    firstCenterLayout->addSpacing(10);
    firstCenterLayout->addWidget(describeLabel);
    firstCenterLayout->addSpacing(8);
    firstCenterLayout->setSpacing(0);
    firstCenterLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *centerWidget = new QWidget(this);
    centerWidget->setLayout(firstCenterLayout);

    QLabel *iconNoDiskLabel =new QLabel(this);
    QIcon iconNoDisk = Common::getIcon("nodisk");
    iconNoDiskLabel->setPixmap(iconNoDisk.pixmap(QSize(136, 136)));
    iconNoDiskLabel->setAlignment(Qt::AlignCenter);
    iconNoDiskLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QLabel *noDiskLabel = new QLabel(this);
    noDiskLabel->setText(tr("No disks or partitions available"));
    noDiskLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    noDiskLabel->setAlignment(Qt::AlignCenter);
    noDiskLabel->setFont(font1);
    noDiskLabel->setPalette(palette2);

    QVBoxLayout *noDiskLayout = new QVBoxLayout;
    noDiskLayout->addStretch();
    noDiskLayout->addWidget(iconNoDiskLabel, 0, Qt::AlignCenter);
    noDiskLayout->addSpacing(10);
    noDiskLayout->addWidget(noDiskLabel, 0, Qt::AlignCenter);
    noDiskLayout->addStretch();
    noDiskLayout->setSpacing(0);
    noDiskLayout->setContentsMargins(0, 0, 0, 0);

    DFrame *noDiskFrame = new DFrame(this);
    noDiskFrame->setBackgroundRole(DPalette::ItemBackground);
    noDiskFrame->setLayout(noDiskLayout);

    QVBoxLayout *noDiskMainLayout = new QVBoxLayout;
    noDiskMainLayout->addWidget(noDiskFrame);
    noDiskMainLayout->setSpacing(0);
    noDiskMainLayout->setContentsMargins(10, 0, 10, 20);

    QWidget *noDiskWidget = new QWidget(this);
    noDiskWidget->setLayout(noDiskMainLayout);

    m_firstCenterStackedWidget = new DStackedWidget(this);
    m_firstCenterStackedWidget->addWidget(centerWidget);
    m_firstCenterStackedWidget->addWidget(noDiskWidget);
    m_firstCenterStackedWidget->setCurrentIndex(0);

    m_firstCancelButton = new DPushButton(this);
    m_firstCancelButton->setText(tr("Cancel", "button"));
    m_firstCancelButton->setAccessibleName("firstCancel");
    m_nextButton = new DSuggestButton(this);
    m_nextButton->setText(tr("Next"));
    m_nextButton->setAccessibleName("next");
    m_nextButton->setDisabled(true);

    QHBoxLayout *firstButtonLayout = new QHBoxLayout;
    firstButtonLayout->addSpacing(87);
    firstButtonLayout->addWidget(m_firstCancelButton);
    firstButtonLayout->addSpacing(10);
    firstButtonLayout->addWidget(m_nextButton);
    firstButtonLayout->addSpacing(87);
    firstButtonLayout->setSpacing(0);
    firstButtonLayout->setContentsMargins(0, 0, 0, 10);

    QVBoxLayout *firstLayout = new QVBoxLayout;
    firstLayout->addWidget(subTitleLabel);
    firstLayout->addSpacing(10);
    firstLayout->addWidget(m_firstCenterStackedWidget);
    firstLayout->addLayout(firstButtonLayout);
    firstLayout->setSpacing(0);
    firstLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *firstWidget = new QWidget(this);
    firstWidget->setLayout(firstLayout);

    // 第二个页面
    DLabel *selectedTitleLabel = new DLabel(tr("Selected disks/partitions"), this);
    selectedTitleLabel->setFont(fontSubTitle);
    selectedTitleLabel->setPalette(paletteSubTitle);
    selectedTitleLabel->setAlignment(Qt::AlignBottom);

    m_addButton = new DPushButton(this);
    QIcon iconAdd = Common::getIcon("add");
    m_addButton->setIcon(iconAdd);
    m_addButton->setAccessibleName("addButton");
    m_addButton->setFixedSize(QSize(36, 36));
    m_addButton->setIconSize(QSize(16, 16));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(selectedTitleLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_addButton);
    topLayout->setSpacing(0);
    topLayout->setContentsMargins(10, 0, 0, 0);

    m_selectedFrame = new DFrame(this);
    paletteBackground.setColor(QPalette::Base, QColor(0, 0, 0, 0));
    m_selectedFrame->setFixedSize(760, 277);
    m_selectedFrame->setPalette(paletteBackground);
    m_selectedFrame->setFrameShadow(QFrame::Plain);
    m_selectedFrame->setFrameShape(QFrame::NoFrame);
    m_selectedFrame->setContentsMargins(0, 0, 0, 0);
    m_selectedFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_selectedScrollArea = new DScrollArea;
    m_selectedScrollArea->setFrameShadow(QFrame::Plain);
    m_selectedScrollArea->setFrameShape(QFrame::NoFrame);
    m_selectedScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
    m_selectedScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖向滚动条

    QVBoxLayout *selectedScrollAreaLayout = new QVBoxLayout;
    selectedScrollAreaLayout->addWidget(m_selectedScrollArea);
    selectedScrollAreaLayout->setSpacing(0);
    selectedScrollAreaLayout->setContentsMargins(0, 0, 0, 0);
    m_selectedFrame->setLayout(selectedScrollAreaLayout);

    DLabel *selectSpaceLabel = new DLabel(tr("Set VG capacity"), this);
    selectSpaceLabel->setFont(font2);
    selectSpaceLabel->setPalette(palette2);
    selectSpaceLabel->setAlignment(Qt::AlignCenter);

    m_selectSpaceLabel = new DLabel("(1-100GiB)", this);
    m_selectSpaceLabel->setFont(font1);
    m_selectSpaceLabel->setPalette(palette1);
    m_selectSpaceLabel->setAccessibleName("range");
    m_selectSpaceLabel->setAlignment(Qt::AlignCenter);

    m_selectSpaceLineEdit = new DLineEdit(this);
    m_selectSpaceLineEdit->setFixedSize(182, 36);
    m_selectSpaceLineEdit->setAccessibleName("vgSize");
    m_selectSpaceLineEdit->lineEdit()->setPlaceholderText("1-100");
    m_selectSpaceLineEdit->setText("100");


    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType) {
        m_selectSpaceLineEdit->setStyleSheet("background:rgba(0, 0, 0, 8)");
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        m_selectSpaceLineEdit->setStyleSheet("background:rgba(255, 255, 255, 8)");
    }

    m_selectSpaceComboBox = new DComboBox(this);
    m_selectSpaceComboBox->setFixedSize(100, 36);
    m_selectSpaceComboBox->addItem("MiB");
    m_selectSpaceComboBox->addItem("GiB");
    m_selectSpaceComboBox->addItem("TiB");
    m_selectSpaceComboBox->setCurrentText("GiB");
    m_selectSpaceComboBox->setAccessibleName("vgUnit");

    QHBoxLayout *selectSpaceLayout = new QHBoxLayout;
    selectSpaceLayout->addWidget(selectSpaceLabel);
    selectSpaceLayout->addSpacing(5);
    selectSpaceLayout->addWidget(m_selectSpaceLabel);
    selectSpaceLayout->addStretch();
    selectSpaceLayout->addWidget(m_selectSpaceLineEdit);
    selectSpaceLayout->addSpacing(10);
    selectSpaceLayout->addWidget(m_selectSpaceComboBox);
    selectSpaceLayout->setSpacing(0);
    selectSpaceLayout->setContentsMargins(10, 6, 10, 6);

    DFrame *checkSpaceFrame = new DFrame(this);
    checkSpaceFrame->setFixedSize(760, 48);
    checkSpaceFrame->setLineWidth(0);
    checkSpaceFrame->setBackgroundRole(DPalette::ItemBackground);
    checkSpaceFrame->setLayout(selectSpaceLayout);

    m_selectedSpaceLabel = new DLabel(tr("Capacity selected: %1"), this);
    m_selectedSpaceLabel->setFont(font1);
    m_selectedSpaceLabel->setPalette(palette1);
    m_selectedSpaceLabel->setAccessibleName("selectedSpace");
    m_selectedSpaceLabel->setAlignment(Qt::AlignCenter | Qt::AlignBottom);

    m_selectSpaceStackedWidget = new DStackedWidget(this);
    m_selectSpaceStackedWidget->addWidget(checkSpaceFrame);
    m_selectSpaceStackedWidget->addWidget(m_selectedSpaceLabel);
    m_selectSpaceStackedWidget->setCurrentIndex(0);

    QVBoxLayout *secondCenterLayout = new QVBoxLayout;
    secondCenterLayout->addLayout(topLayout);
    secondCenterLayout->addSpacing(10);
    secondCenterLayout->addWidget(m_selectedFrame);
    secondCenterLayout->addSpacing(15);
    secondCenterLayout->addWidget(m_selectSpaceStackedWidget);
    secondCenterLayout->setSpacing(0);
    secondCenterLayout->setContentsMargins(10, 2, 10, 0);

    QWidget *secondCenterWidget = new QWidget(this);
    secondCenterWidget->setLayout(secondCenterLayout);

    QLabel *iconNoInfoLabel =new QLabel(this);
    QIcon iconNoInfo = Common::getIcon("nodisk");
    iconNoInfoLabel->setPixmap(iconNoInfo.pixmap(QSize(136, 136)));
    iconNoInfoLabel->setAlignment(Qt::AlignCenter);
    iconNoInfoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QLabel *noInfoLabel = new QLabel(this);
    noInfoLabel->setText(tr("Choose one disk or partition at least"));
    noInfoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    noInfoLabel->setAlignment(Qt::AlignCenter);
    noInfoLabel->setFont(font1);
    noInfoLabel->setPalette(palette2);

    QVBoxLayout *noInfoLayout = new QVBoxLayout;
    noInfoLayout->addStretch();
    noInfoLayout->addWidget(iconNoInfoLabel, 0, Qt::AlignCenter);
    noInfoLayout->addSpacing(10);
    noInfoLayout->addWidget(noInfoLabel, 0, Qt::AlignCenter);
    noInfoLayout->addStretch();
    noInfoLayout->setSpacing(0);
    noInfoLayout->setContentsMargins(0, 0, 0, 0);

    DFrame *noInfoFrame = new DFrame(this);
    noInfoFrame->setBackgroundRole(DPalette::ItemBackground);
    noInfoFrame->setLayout(noInfoLayout);

    QVBoxLayout *noInfoMainLayout = new QVBoxLayout;
    noInfoMainLayout->addWidget(noInfoFrame);
    noInfoMainLayout->setSpacing(0);
    noInfoMainLayout->setContentsMargins(10, 6, 10, 10);

    QWidget *noInfoWidget = new QWidget(this);
    noInfoWidget->setLayout(noInfoMainLayout);

    m_selectedStackedWidget = new DStackedWidget(this);
    m_selectedStackedWidget->addWidget(secondCenterWidget);
    m_selectedStackedWidget->addWidget(noInfoWidget);
    m_selectedStackedWidget->setCurrentIndex(1);

    m_secondCancelButton = new DPushButton(this);
    m_secondCancelButton->setText(tr("Cancel", "button"));
    m_secondCancelButton->setAccessibleName("secondCancel");
    m_previousButton = new DPushButton(this);
    m_previousButton->setText(tr("Previous"));
    m_previousButton->setAccessibleName("previous");
    m_doneButton = new DSuggestButton(this);
    m_doneButton->setText(tr("Done", "button"));
    m_doneButton->setAccessibleName("done");

    QHBoxLayout *secondButtonLayout = new QHBoxLayout;
    secondButtonLayout->addSpacing(110);
    secondButtonLayout->addWidget(m_secondCancelButton);
    secondButtonLayout->addSpacing(10);
    secondButtonLayout->addWidget(m_previousButton);
    secondButtonLayout->addSpacing(10);
    secondButtonLayout->addWidget(m_doneButton);
    secondButtonLayout->addSpacing(110);
    secondButtonLayout->setSpacing(0);
    secondButtonLayout->setContentsMargins(0, 0, 0, 10);

    QVBoxLayout *secondLayout = new QVBoxLayout;
    secondLayout->addWidget(m_selectedStackedWidget);
    secondLayout->addSpacing(10);
    secondLayout->addLayout(secondButtonLayout);
    secondLayout->setSpacing(0);
    secondLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *secondWidget = new QWidget(this);
    secondWidget->setLayout(secondLayout);

    m_loadingWidget = new QWidget(this);

    m_stackedWidget = new DStackedWidget(this);
    m_stackedWidget->addWidget(firstWidget);
    m_stackedWidget->addWidget(secondWidget);
    m_stackedWidget->addWidget(m_loadingWidget);
    m_stackedWidget->setCurrentIndex(0);

    QVBoxLayout *mainLayout = new QVBoxLayout(m_mainFrame);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(m_stackedWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
}

void CreateVGWidget::initConnection()
{
    connect(m_nextButton, &DSuggestButton::clicked, this, &CreateVGWidget::onNextButtonClicked);
    connect(m_addButton, &DPushButton::clicked, this, &CreateVGWidget::onPreviousButtonClicked);
    connect(m_previousButton, &DPushButton::clicked, this, &CreateVGWidget::onPreviousButtonClicked);
    connect(m_doneButton, &DPushButton::clicked, this, &CreateVGWidget::onDoneButtonClicked);
    connect(m_firstCancelButton, &DPushButton::clicked, this, &CreateVGWidget::onCancelButtonClicked);
    connect(m_secondCancelButton, &DPushButton::clicked, this, &CreateVGWidget::onCancelButtonClicked);
    connect(m_selectSpaceComboBox, static_cast<void (DComboBox:: *)(const int)>(&DComboBox::currentIndexChanged),
            this, &CreateVGWidget::onCurrentIndexChanged);
    connect(m_selectSpaceLineEdit, &DLineEdit::textChanged, this, &CreateVGWidget::onTextChanged);
    connect(DMDbusHandler::instance(), &DMDbusHandler::vgCreateMessage, this, &CreateVGWidget::onVGCreateMessage);
}

void CreateVGWidget::showLoadingWidget(const QString &vgName)
{
    m_stackedWidget->setCurrentIndex(2);
    DPalette palette1;
    palette1.setColor(DPalette::TextTitle, QColor("#001A2E"));

    DPalette palette2;
    palette2.setColor(DPalette::ButtonText, QColor("#414D68"));

    QFont font1;
    font1.setWeight(QFont::Normal);
    font1.setFamily("Source Han Sans");
    font1.setPixelSize(12);

    QFont font2;
    font2.setWeight(QFont::Medium);
    font2.setFamily("Source Han Sans");
    font2.setPixelSize(14);

    m_waterLoadingWidget = new WaterLoadingWidget(this);
    m_waterLoadingWidget->setFixedSize(98, 98);

    QLabel *waitLable = new QLabel(this);
    waitLable->setText(tr("Creating..."));
    waitLable->setAlignment(Qt::AlignCenter);
    waitLable->setFont(font1);
    waitLable->setPalette(palette1);

    QVBoxLayout *layout = new QVBoxLayout;
    for (int i = 0; i < m_curSeclectData.count(); ++++i) {
        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignCenter);
        label->setFont(font2);
        label->setPalette(palette2);

        if (i == (m_curSeclectData.count() - 1)) {
            label->setText(QString("%1").arg(m_curSeclectData.at(i).m_partitionPath));
        } else {
            QString path1;
            QString path2;
            PVInfoData pvInfoData1 = m_curSeclectData.at(i);
            PVInfoData pvInfoData2 = m_curSeclectData.at(i + 1);

            if (pvInfoData1.m_disktype == "unrecognized") {
                path1 = pvInfoData1.m_diskPath;
            } else {
                path1 = pvInfoData1.m_partitionPath;
            }

            if (pvInfoData2.m_disktype == "unrecognized") {
                path2 = pvInfoData2.m_diskPath;
            } else {
                path2 = pvInfoData2.m_partitionPath;
            }

            if (i == (m_curSeclectData.count() - 2)) {
                label->setText(QString("%1, %2").arg(path1).arg(path2));
            } else {
                label->setText(QString("%1, %2, ").arg(path1).arg(path2));
            }
        }

        layout->addWidget(label, 0, Qt::AlignCenter);
    }

    layout->addStretch();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    DFrame *frame = new DFrame;
    DPalette paletteBase;
    paletteBase.setColor(QPalette::Base, QColor(0, 0, 0, 0));
    frame->setPalette(paletteBase);
    frame->setFrameShadow(QFrame::Plain);
    frame->setFrameShape(QFrame::NoFrame);

    QLabel *selectLable = new QLabel(this);
    selectLable->setText(tr("Selected disks and partitions:"));
    selectLable->setAlignment(Qt::AlignCenter);
    selectLable->setFont(font2);
    selectLable->setPalette(palette2);

    int count = m_curSeclectData.count() / 2 + m_curSeclectData.count() % 2;
    if (count > 5) {
        QWidget *widget = new QWidget;
        widget->setLayout(layout);
        widget->setFixedSize(600, count * 23 - 8);

        DScrollArea *scrollArea = new DScrollArea;
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
        scrollArea->setWidget(widget);

        QVBoxLayout *scrollAreaLayout = new QVBoxLayout;
        scrollAreaLayout->addWidget(selectLable, 0, Qt::AlignCenter);
        scrollAreaLayout->addSpacing(5);
        scrollAreaLayout->addWidget(scrollArea);
        scrollAreaLayout->setSpacing(0);
        scrollAreaLayout->setContentsMargins(0, 0, 0, 0);

        frame->setFixedSize(600, 124);
        frame->setLayout(scrollAreaLayout);
    } else {
        layout->insertWidget(0, selectLable, 0, Qt::AlignCenter);

        frame->setFixedSize(600, (count + 1) * 23);
        frame->setLayout(layout);
    }

    QLabel *vgSizeLabel = new QLabel(this);
    vgSizeLabel->setAlignment(Qt::AlignCenter);
    vgSizeLabel->setFont(font2);
    vgSizeLabel->setPalette(palette2);

    if (m_selectSpaceStackedWidget->currentIndex() == 1) {
        vgSizeLabel->setText(tr("VG capacity:%1").arg(QString::number(m_minSize, 'f', 2)) + m_selectSpaceComboBox->currentText());
    } else {
        vgSizeLabel->setText(tr("VG capacity:%1").arg(m_selectSpaceLineEdit->text() + m_selectSpaceComboBox->currentText()));
    }

    QLabel *vgNameLabel = new QLabel(this);
    vgNameLabel->setAlignment(Qt::AlignCenter);
    vgNameLabel->setText(tr("VG name:%1").arg(vgName));
    vgNameLabel->setFont(font2);
    vgNameLabel->setPalette(palette2);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addSpacing(26);
    mainLayout->addWidget(m_waterLoadingWidget, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(waitLable, 0, Qt::AlignCenter);
    mainLayout->addSpacing(40);
    mainLayout->addWidget(frame, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(vgSizeLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(vgNameLabel, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    m_loadingWidget->setLayout(mainLayout);

    DWindowCloseButton *button = findChild<DWindowCloseButton *>("DTitlebarDWindowCloseButton");
    if (button != nullptr) {
        button->setDisabled(true);
        button->hide();
    }


    m_waterLoadingWidget->setStartTime(1000);
}

QString CreateVGWidget::getVGName()
{
    LVMInfo lvmInfo = DMDbusHandler::instance()->probLVMInfo();
    QList<QString> lvmNameList = lvmInfo.m_vgInfo.keys();
    QString name = QString("vg%1").arg(lvmNameList.count() + 1, 2, 10, QLatin1Char('0'));
    for (int i = 0; i <= lvmNameList.count(); i++) {
        name = QString("vg%1").arg(i + 1, 2, 10, QLatin1Char('0'));
        if (-1 == lvmNameList.indexOf(name)) {
            break;
        }
    }
    return name;
}

void CreateVGWidget::onNextButtonClicked()
{
    m_stackedWidget->setCurrentIndex(1);
    updateSelectedData();
}

void CreateVGWidget::onPreviousButtonClicked()
{
    m_stackedWidget->setCurrentIndex(0);
    updateData();
}

void CreateVGWidget::onDoneButtonClicked()
{
    QString vgName = getVGName();
    QList<PVData> pvDataList;

    for (int i = 0; i < m_curSeclectData.count(); i++) {
        PVInfoData infoData = m_curSeclectData.at(i);

        PVData pvData;
        pvData.m_diskPath = infoData.m_diskPath;
        pvData.m_startSector = infoData.m_sectorStart;
        pvData.m_endSector = infoData.m_sectorEnd;
        pvData.m_sectorSize = static_cast<int>(infoData.m_sectorSize);
        pvData.m_pvAct = LVMAction::LVM_ACT_ADDPV;

        if (infoData.m_disktype == "unrecognized") {
            pvData.m_devicePath = infoData.m_diskPath;
            pvData.m_type = LVMDevType::LVM_DEV_DISK;
        } else {
            if (infoData.m_partitionPath == "unallocated") {
                pvData.m_devicePath = infoData.m_diskPath;
                pvData.m_type = LVMDevType::LVM_DEV_UNALLOCATED_PARTITION;
            } else {
                pvData.m_devicePath = infoData.m_partitionPath;
                pvData.m_type = LVMDevType::LVM_DEV_PARTITION;
            }

        }

        pvDataList.append(pvData);
    }

    DMDbusHandler::instance()->createVG(vgName, pvDataList, m_curSize);
    showLoadingWidget(vgName);
}

void CreateVGWidget::onCancelButtonClicked()
{
    close();
}

void CreateVGWidget::updateData()
{
    QList<DeviceInfo> lstDeviceInfo = availableDiskData();
    if (lstDeviceInfo.count() == 0) {
        m_firstCenterStackedWidget->setCurrentIndex(1);
        m_nextButton->setDisabled(true);
    } else {
        QWidget *diskWidget = m_diskScrollArea->findChild<QWidget *>("diskScrollArea");
        if (diskWidget != nullptr) {
            delete diskWidget;
            diskWidget = nullptr;
        }

        m_curDiskItemWidget = nullptr;

        QWidget *partWidget = m_partitionScrollArea->findChild<QWidget *>("partScrollArea");
        if (partWidget != nullptr) {
            delete partWidget;
            partWidget = nullptr;
        }

        m_firstCenterStackedWidget->setCurrentIndex(0);
        QVBoxLayout *diskLayout = new QVBoxLayout;
        for (int i = 0; i < lstDeviceInfo.count(); i++) {
            DeviceInfo info = lstDeviceInfo.at(i);
            QString diskSize = Utils::formatSize(info.m_length, info.m_sectorSize);

            PVInfoData diskInfoData;
            diskInfoData.m_level = DMDbusHandler::DISK;
            diskInfoData.m_disktype = info.m_disktype;
            diskInfoData.m_diskPath = info.m_path;
            diskInfoData.m_diskSize = diskSize;
            diskInfoData.m_partitionPath = "";
            diskInfoData.m_partitionSize = "";
            diskInfoData.m_sectorSize = info.m_sectorSize;
            diskInfoData.m_sectorStart = 0;
            diskInfoData.m_sectorEnd = 0;
            diskInfoData.m_selectStatus = Qt::CheckState::Unchecked;

            QList<PVInfoData> lstPVInfoData;
            lstPVInfoData.clear();
            if (info.m_partition.size() == 1) {
                PartitionInfo partitionInfo = info.m_partition.at(0);
                QString partitionSize = Utils::formatSize(partitionInfo.m_sectorEnd - partitionInfo.m_sectorStart + 1,
                                                          partitionInfo.m_sectorSize);
                if (info.m_disktype != "unrecognized") {
                    diskInfoData.m_sectorStart = partitionInfo.m_sectorStart;
                    diskInfoData.m_sectorEnd = partitionInfo.m_sectorEnd;

                    PVInfoData partInfoData;
                    partInfoData.m_level = DMDbusHandler::PARTITION;
                    partInfoData.m_disktype = info.m_disktype;
                    partInfoData.m_diskPath = partitionInfo.m_devicePath;
                    partInfoData.m_diskSize = "";
                    partInfoData.m_partitionPath = partitionInfo.m_path;
                    partInfoData.m_partitionSize = partitionSize;
                    partInfoData.m_sectorSize = partitionInfo.m_sectorSize;
                    partInfoData.m_sectorStart = partitionInfo.m_sectorStart;
                    partInfoData.m_sectorEnd = partitionInfo.m_sectorEnd;
                    partInfoData.m_selectStatus = Qt::CheckState::Unchecked;

                    for (int i = 0; i < m_curSeclectData.count(); i++) {
                        PVInfoData infoData = m_curSeclectData.at(i);
                        if (partitionInfo.m_path == infoData.m_partitionPath && partitionInfo.m_sectorStart == infoData.m_sectorStart &&
                                partitionInfo.m_sectorEnd == infoData.m_sectorEnd) {
                                diskInfoData.m_selectStatus = Qt::CheckState::Checked;
                                partInfoData.m_selectStatus = Qt::CheckState::Checked;
                            break;
                        }
                    }

                    lstPVInfoData.append(partInfoData);
                } else {
                    diskInfoData.m_sectorStart = 0;
                    diskInfoData.m_sectorEnd = info.m_length - 1;

                    for (int i = 0; i < m_curSeclectData.count(); i++) {
                        PVInfoData infoData = m_curSeclectData.at(i);
                        if (partitionInfo.m_devicePath == infoData.m_diskPath && partitionInfo.m_sectorStart == infoData.m_sectorStart &&
                                partitionInfo.m_sectorEnd == infoData.m_sectorEnd) {
                                diskInfoData.m_selectStatus = Qt::CheckState::Checked;
                            break;
                        }
                    }
                }
            } else {
                int checkCount = 0;
                for (int i = 0; i < info.m_partition.size(); i++) {
                    PartitionInfo partitionInfo = info.m_partition.at(i);
                    QString partitionSize = Utils::formatSize(partitionInfo.m_sectorEnd - partitionInfo.m_sectorStart + 1,
                                                              partitionInfo.m_sectorSize);

                    PVInfoData partInfoData;
                    partInfoData.m_level = DMDbusHandler::PARTITION;
                    partInfoData.m_disktype = info.m_disktype;
                    partInfoData.m_diskPath = partitionInfo.m_devicePath;
                    partInfoData.m_diskSize = "";
                    partInfoData.m_partitionPath = partitionInfo.m_path;
                    partInfoData.m_partitionSize = partitionSize;
                    partInfoData.m_sectorSize = partitionInfo.m_sectorSize;
                    partInfoData.m_sectorStart = partitionInfo.m_sectorStart;
                    partInfoData.m_sectorEnd = partitionInfo.m_sectorEnd;
                    partInfoData.m_selectStatus = Qt::CheckState::Unchecked;

                    for (int j = 0; j < m_curSeclectData.count(); j++) {
                        PVInfoData infoData = m_curSeclectData.at(j);
                        if (partitionInfo.m_path == infoData.m_partitionPath && partitionInfo.m_sectorStart == infoData.m_sectorStart &&
                                partitionInfo.m_sectorEnd == infoData.m_sectorEnd) {
                                partInfoData.m_selectStatus = Qt::CheckState::Checked;
                                checkCount++;
                            break;
                        }
                    }

                    lstPVInfoData.append(partInfoData);
                }

                if (checkCount == 0) {
                    diskInfoData.m_selectStatus = Qt::CheckState::Unchecked;
                } else if (checkCount == lstPVInfoData.count()){
                    diskInfoData.m_selectStatus = Qt::CheckState::Checked;
                } else {
                    diskInfoData.m_selectStatus = Qt::CheckState::PartiallyChecked;
                }
            }

            SelectPVItemWidget *selectPVItemWidget = new SelectPVItemWidget(diskInfoData);
            selectPVItemWidget->setData(lstPVInfoData);

            if (lstDeviceInfo.count() == 1) {
                selectPVItemWidget->setMode(true, true, true, true);
            } else {
                if (i == 0) {
                    selectPVItemWidget->setMode(true, false, true, false);
                } else if (i == lstDeviceInfo.count() - 1) {
                    selectPVItemWidget->setMode(false, true, false, true);
                } else {
                    selectPVItemWidget->setMode(false, false, false, false);
                }
            }

            connect(selectPVItemWidget, &SelectPVItemWidget::selectItem, this, &CreateVGWidget::onDiskItemClicked);
            connect(selectPVItemWidget, &SelectPVItemWidget::checkBoxStateChange, this, &CreateVGWidget::onDiskCheckBoxStateChange);

            diskLayout->addWidget(selectPVItemWidget);
        }

        diskLayout->addStretch();
        diskLayout->setSpacing(1);
        diskLayout->setContentsMargins(0, 0, 0, 0);

        QWidget *widget = new QWidget;
        widget->setObjectName("diskScrollArea");
        widget->setFixedSize(370, lstDeviceInfo.count() * 37);
        widget->setLayout(diskLayout);
        m_diskScrollArea->setWidget(widget);
    }
}

QList<DeviceInfo> CreateVGWidget::availableDiskData()
{
    QList<DeviceInfo> lstDeviceInfo;
    lstDeviceInfo.clear();

    DeviceInfoMap deviceInfoMap = DMDbusHandler::instance()->probDeviceInfo();
    QMap<QString, QString> isJoinAllVG = DMDbusHandler::instance()->getIsJoinAllVG();

    for (auto devInfo = deviceInfoMap.begin(); devInfo != deviceInfoMap.end(); devInfo++) {
        DeviceInfo info = devInfo.value();

        // 排除已加入VG的磁盘以及分区表错误的磁盘
        if (isJoinAllVG.value(info.m_path) == "false") {//还需加上磁盘分区表是否错误的判断
            PartitionVec lstNewPartition;
            lstNewPartition.clear();
            bool isAvailable = true;

            if (info.m_partition.size() == 1) {
                PartitionInfo partitionInfo = info.m_partition.at(0);

                QString mountpoints = "";
                for (int i = 0; i < partitionInfo.m_mountPoints.size(); i++) {
                    mountpoints += partitionInfo.m_mountPoints[i];
                }

                // 只有一个分区时，排除已加入VG的分区、被挂载的分区、分区大小小于100MiB的分区
                double partitionSize = Utils::sectorToUnit(partitionInfo.m_sectorEnd - partitionInfo.m_sectorStart + 1,
                                                           partitionInfo.m_sectorSize, UNIT_MIB);
                if ((1 != partitionInfo.m_vgFlag) && (mountpoints.isEmpty()) && (partitionSize > 100)) {
                    PartitionInfo newPartitionInfo = partitionInfo;
                    lstNewPartition.append(newPartitionInfo);
                } else {
                    isAvailable = false;
                }
            } else {
                int partitionCount = 0;
                for (int i = 0; i < info.m_partition.size(); i++) {
                    if (info.m_partition.at(i).m_path != "unallocated") {
                        partitionCount++;
                    }
                }

                for (int i = 0; i < info.m_partition.size(); i++) {
                    PartitionInfo partitionInfo = info.m_partition.at(i);

                    QString mountpoints = "";
                    for (int j = 0; j < partitionInfo.m_mountPoints.size(); j++) {
                        mountpoints += partitionInfo.m_mountPoints[j];
                    }

                    // 排除系统磁盘
                    if (mountpoints == "/boot/efi" || mountpoints == "/boot" || mountpoints == "/"
                            || mountpoints == "/recovery" || partitionInfo.m_flag == 4) {
                        isAvailable = false;
                        break;
                    }

                    // 排除已加入VG的分区、被挂载的分区、分区大小小于100MiB的分区
                    double partitionSize = Utils::sectorToUnit(partitionInfo.m_sectorEnd - partitionInfo.m_sectorStart + 1,
                                                               partitionInfo.m_sectorSize, UNIT_MIB);
                    if ((1 != partitionInfo.m_vgFlag) && (mountpoints.isEmpty()) && (partitionSize > 100)) {
                        // 排除不能创建新分区的磁盘空闲空间
                        if (partitionInfo.m_path == "unallocated") {
                            if (partitionCount >= info.m_maxPrims) {
                                continue;
                            }
                        }

                        // 此处为各种条件排除后可创建VG的分区
                        PartitionInfo newPartitionInfo = partitionInfo;
                        lstNewPartition.append(newPartitionInfo);
                    }
                }
            }

            if (isAvailable) {
                DeviceInfo newDeviceInfo = info;
                newDeviceInfo.m_partition = lstNewPartition;

                lstDeviceInfo.append(newDeviceInfo);
            }
        }
    }

    return lstDeviceInfo;
}

void CreateVGWidget::onDiskItemClicked()
{
    // 当前选中行是否为已选中行
    SelectPVItemWidget *selectPVItemWidget = qobject_cast<SelectPVItemWidget *>(sender());
    if (m_curDiskItemWidget == selectPVItemWidget) {
        return;
    }

    // 选中其他行时
    if (nullptr != m_curDiskItemWidget) {
        m_curDiskItemWidget->setChecked(false);

        QWidget *partWidget = m_partitionScrollArea->findChild<QWidget *>("partScrollArea");
        if (partWidget != nullptr) {
            delete partWidget;
            partWidget = nullptr;
        }
    }

    selectPVItemWidget->setChecked(true);
    QList<PVInfoData> lstData = selectPVItemWidget->getData();
    m_curDiskItemWidget = selectPVItemWidget;

    updatePartitionData(lstData);
}

void CreateVGWidget::updatePartitionData(const QList<PVInfoData> &lstData)
{
    if (lstData.count() == 0) {
        m_partitionStackedWidget->setCurrentIndex(1);
    } else {
        m_partitionStackedWidget->setCurrentIndex(0);

        QVBoxLayout *partitionLayout = new QVBoxLayout;
        for (int i = 0; i < lstData.count(); i++) {
            PVInfoData pvInfoData = lstData.at(i);

            SelectPVItemWidget *selectPVItemWidget = new SelectPVItemWidget(pvInfoData);
            if (lstData.count() == 1) {
                selectPVItemWidget->setMode(true, true, true, true);
            } else {
                if (i == 0) {
                    selectPVItemWidget->setMode(true, false, true, false);
                } else if (i == lstData.count() - 1) {
                    selectPVItemWidget->setMode(false, true, false, true);
                } else {
                    selectPVItemWidget->setMode(false, false, false, false);
                }
            }

            switch (pvInfoData.m_selectStatus) {
            case Qt::CheckState::Unchecked: {
                selectPVItemWidget->setCheckBoxState(Qt::CheckState::Unchecked);
                break;
            }
            case Qt::CheckState::Checked: {
                selectPVItemWidget->setCheckBoxState(Qt::CheckState::Checked);
                break;
            }
            default:
                break;
            }

            connect(selectPVItemWidget, &SelectPVItemWidget::selectItem, this, &CreateVGWidget::onDiskItemClicked);
            connect(selectPVItemWidget, &SelectPVItemWidget::checkBoxStateChange, this, &CreateVGWidget::onPartitionCheckBoxStateChange);

            partitionLayout->addWidget(selectPVItemWidget);
        }

        partitionLayout->addStretch();
        partitionLayout->setSpacing(1);
        partitionLayout->setContentsMargins(0, 0, 0, 0);

        QWidget *widget = new QWidget;
        widget->setObjectName("partScrollArea");
        widget->setFixedSize(370, lstData.count() * 37);
        widget->setLayout(partitionLayout);
        m_partitionScrollArea->setWidget(widget);
    }
}

void CreateVGWidget::onDiskCheckBoxStateChange(int state)
{
    // 若为半选状态置为选中状态
    if (state == Qt::CheckState::PartiallyChecked) {
        SelectPVItemWidget *selectPVItemWidget = qobject_cast<SelectPVItemWidget *>(sender());
        selectPVItemWidget->setCheckBoxState(Qt::CheckState::Checked, true);
    } else {
        if (state == Qt::CheckState::Unchecked) {
            // 磁盘取消选中
            QList<SelectPVItemWidget*> lstPartitionItem = m_partitionScrollArea->findChildren<SelectPVItemWidget*>();
            if (lstPartitionItem.count() == 0) {
                onDiskItemClicked();
            } else {
                SelectPVItemWidget *selectPVItemWidget = qobject_cast<SelectPVItemWidget *>(sender());
                if (m_curDiskItemWidget == selectPVItemWidget) {
                    for (int i = 0; i < lstPartitionItem.count(); i++) {
                        SelectPVItemWidget *selectPVItemWidget = lstPartitionItem.at(i);
                        selectPVItemWidget->setCheckBoxState(Qt::CheckState::Unchecked);
                    }
                } else {
                    onDiskItemClicked();
                }
            }

            // 当前取消磁盘是否没有分区表
            if (m_curDiskItemWidget->getCurInfo().m_disktype == "unrecognized") {
                PVInfoData pvData = m_curDiskItemWidget->getCurInfo();
                for (int i = 0; i < m_curSeclectData.count(); i++) {
                    PVInfoData infoData = m_curSeclectData.at(i);
                    if (pvData.m_diskPath == infoData.m_diskPath && pvData.m_sectorStart == infoData.m_sectorStart &&
                            pvData.m_sectorEnd == infoData.m_sectorEnd) {
                        m_curSeclectData.removeAt(i);
                        break;
                    }
                }
            } else {
                QList<PVInfoData> lstData = m_curDiskItemWidget->getData();
                for (int i = 0; i < lstData.count(); i++) {
                    PVInfoData pvData = lstData.at(i);

                    for (int j = 0; j < m_curSeclectData.count(); j++) {
                        PVInfoData infoData = m_curSeclectData.at(j);
                        if (pvData.m_partitionPath == infoData.m_partitionPath && pvData.m_sectorStart == infoData.m_sectorStart &&
                                pvData.m_sectorEnd == infoData.m_sectorEnd) {
                            m_curSeclectData.removeAt(j);
                            break;
                        }
                    }
                }
            }

            if (m_curSeclectData.count() == 0) {
                m_seclectedLabel->setText(tr("Capacity selected: %1").arg("0GiB"));
                m_nextButton->setDisabled(true);
            } else {
                double sumSize = 0;
                for (int i = 0; i < m_curSeclectData.count(); i++) {
                    PVInfoData infoData = m_curSeclectData.at(i);
                    sumSize += Utils::sectorToUnit(infoData.m_sectorEnd - infoData.m_sectorStart + 1,
                                                   infoData.m_sectorSize, SIZE_UNIT::UNIT_GIB);
                }

                m_seclectedLabel->setText(tr("Capacity selected: %1").arg(QString::number(sumSize, 'f', 2)) + "GiB");
            }
        } else if (state == Qt::CheckState::Checked) {
            // 磁盘选中
            QList<SelectPVItemWidget*> lstPartitionItem = m_partitionScrollArea->findChildren<SelectPVItemWidget*>();
            if (lstPartitionItem.count() == 0) {
                onDiskItemClicked();
            } else {
                SelectPVItemWidget *selectPVItemWidget = qobject_cast<SelectPVItemWidget *>(sender());
                if (m_curDiskItemWidget == selectPVItemWidget) {
                    for (int i = 0; i < lstPartitionItem.count(); i++) {
                        SelectPVItemWidget *selectPVItemWidget = lstPartitionItem.at(i);
                        selectPVItemWidget->setCheckBoxState(Qt::CheckState::Checked);
                    }
                } else {
                    onDiskItemClicked();
                }
            }

            // 当前取消磁盘是否没有分区表
            if (m_curDiskItemWidget->getCurInfo().m_disktype == "unrecognized") {
                m_curSeclectData << m_curDiskItemWidget->getCurInfo();
            } else {
                QList<PVInfoData> lstData = m_curDiskItemWidget->getData();
                for (int i = 0; i < lstData.count(); i++) {
                    PVInfoData pvData = lstData.at(i);

                    bool isSameData = false;
                    for (int j = 0; j < m_curSeclectData.count(); j++) {
                        PVInfoData infoData = m_curSeclectData.at(j);
                        if (pvData.m_partitionPath == infoData.m_partitionPath && pvData.m_sectorStart == infoData.m_sectorStart &&
                                pvData.m_sectorEnd == infoData.m_sectorEnd) {
                            isSameData = true;
                            break;
                        }
                    }

                    if (!isSameData) {
                        m_curSeclectData << pvData;
                    }
                }
            }

            double sumSize = 0;
            for (int i = 0; i < m_curSeclectData.count(); i++) {
                PVInfoData infoData = m_curSeclectData.at(i);
                sumSize += Utils::sectorToUnit(infoData.m_sectorEnd - infoData.m_sectorStart + 1,
                                               infoData.m_sectorSize, SIZE_UNIT::UNIT_GIB);
            }

            m_seclectedLabel->setText(tr("Capacity selected: %1").arg(QString::number(sumSize, 'f', 2)) + "GiB");
            m_nextButton->setDisabled(false);
        }
    }
}

void CreateVGWidget::onPartitionCheckBoxStateChange(int state)
{
    SelectPVItemWidget *selectPVItemWidget = qobject_cast<SelectPVItemWidget *>(sender());
    PVInfoData pvData = selectPVItemWidget->getCurInfo();
    QList<PVInfoData> lstData = m_curDiskItemWidget->getData();
    bool isPartiallyCheck = false;

    if (state == Qt::CheckState::Unchecked) {
        // 分区取消选中
        for (int i = 0; i < lstData.count(); i++) {
            PVInfoData data = lstData.at(i);
            if (pvData.m_sectorStart == data.m_sectorStart && pvData.m_sectorEnd == data.m_sectorEnd) {
                data.m_selectStatus = state;
                lstData.replace(i, data);
            }

            if (data.m_selectStatus == Qt::CheckState::Checked) {
                isPartiallyCheck = true;
            }
        }

        if (isPartiallyCheck) {
            m_curDiskItemWidget->setCheckBoxState(Qt::CheckState::PartiallyChecked);
        } else {
            m_curDiskItemWidget->setCheckBoxState(Qt::CheckState::Unchecked);
        }

        double sumSize = 0;
        int deleteNumber = -1;
        for (int i = 0; i < m_curSeclectData.count(); i++) {
            PVInfoData infoData = m_curSeclectData.at(i);
            if (pvData.m_partitionPath == infoData.m_partitionPath && pvData.m_sectorStart == infoData.m_sectorStart &&
                    pvData.m_sectorEnd == infoData.m_sectorEnd) {
                deleteNumber = i;
            } else {
                sumSize += Utils::sectorToUnit(infoData.m_sectorEnd - infoData.m_sectorStart + 1,
                                               infoData.m_sectorSize, SIZE_UNIT::UNIT_GIB);
            }
        }

        if (deleteNumber >= 0) {
            m_curSeclectData.removeAt(deleteNumber);
        }

        if (m_curSeclectData.count() == 0) {
            m_seclectedLabel->setText(tr("Capacity selected: %1").arg("0GiB"));
            m_nextButton->setDisabled(true);
        } else {
            m_seclectedLabel->setText(tr("Capacity selected: %1").arg(QString::number(sumSize, 'f', 2)) + "GiB");
        }
    } else {
        // 分区选中
        for (int i = 0; i < lstData.count(); i++) {
            PVInfoData data = lstData.at(i);
            if (pvData.m_sectorStart == data.m_sectorStart && pvData.m_sectorEnd == data.m_sectorEnd ) {
                data.m_selectStatus = state;
                lstData.replace(i, data);
            }

            if (data.m_selectStatus == Qt::CheckState::Unchecked) {
                isPartiallyCheck = true;
            }
        }

        if (isPartiallyCheck) {
            m_curDiskItemWidget->setCheckBoxState(Qt::CheckState::PartiallyChecked);
        } else {
            m_curDiskItemWidget->setCheckBoxState(Qt::CheckState::Checked);
        }

        m_curSeclectData.append(pvData);
        double sumSize = 0;
        for (int i = 0; i < m_curSeclectData.count(); i++) {
            PVInfoData infoData = m_curSeclectData.at(i);
            sumSize += Utils::sectorToUnit(infoData.m_sectorEnd - infoData.m_sectorStart + 1,
                                           infoData.m_sectorSize, SIZE_UNIT::UNIT_GIB);
        }

        m_seclectedLabel->setText(tr("Capacity selected: %1").arg(QString::number(sumSize, 'f', 2)) + "GiB");
        m_nextButton->setDisabled(false);
    }

    m_curDiskItemWidget->setData(lstData);
}

SIZE_UNIT CreateVGWidget::getCurSizeUnit()
{
    SIZE_UNIT type = SIZE_UNIT::UNIT_GIB;
    switch (m_selectSpaceComboBox->currentIndex()) {
    case 0: {
        type = SIZE_UNIT::UNIT_MIB;
        break;
    }
    case 1: {
        type = SIZE_UNIT::UNIT_GIB;
        break;
    }
    case 2: {
        type = SIZE_UNIT::UNIT_TIB;
        break;
    }
    default:
        break;
    }

    return type;
}

void CreateVGWidget::updateSelectedData()
{
    if (m_curSeclectData.count() == 0) {
        m_selectedStackedWidget->setCurrentIndex(1);
        m_doneButton->setDisabled(true);
    } else {
        m_selectedStackedWidget->setCurrentIndex(0);
        m_doneButton->setDisabled(false);

        QWidget *selectedWidget = m_selectedScrollArea->findChild<QWidget *>("selectedScrollArea");
        if (selectedWidget != nullptr) {
            delete selectedWidget;
            selectedWidget = nullptr;
        }

        QVBoxLayout *selectedLayout = new QVBoxLayout;
        bool isUnallocated = false;
        m_minSize = 0;
        m_maxSize = 0;
        m_sumSize = 0;
        SIZE_UNIT type = getCurSizeUnit();

        for (int i = 0; i < m_curSeclectData.count(); i++) {
            PVInfoData pvInfoData = m_curSeclectData.at(i);

            if (pvInfoData.m_partitionPath == "unallocated" || pvInfoData.m_disktype == "unrecognized") {
                isUnallocated = true;
            } else {
                m_minSize += Utils::sectorToUnit(pvInfoData.m_sectorEnd - pvInfoData.m_sectorStart + 1,
                                               pvInfoData.m_sectorSize, type);
            }

            m_maxSize += Utils::sectorToUnit(pvInfoData.m_sectorEnd - pvInfoData.m_sectorStart + 1,
                                             pvInfoData.m_sectorSize, type);

            m_sumSize += (pvInfoData.m_sectorEnd - pvInfoData.m_sectorStart + 1) * pvInfoData.m_sectorSize;

            SelectedItemWidget *selectedItemWidget = new SelectedItemWidget(pvInfoData);
            if (m_curSeclectData.count() == 1) {
                selectedItemWidget->setMode(true, true, true, true);
            } else {
                if (i == 0) {
                    selectedItemWidget->setMode(true, false, true, false);
                } else if (i == m_curSeclectData.count() - 1) {
                    selectedItemWidget->setMode(false, true, false, true);
                } else {
                    selectedItemWidget->setMode(false, false, false, false);
                }
            }

            connect(selectedItemWidget, &SelectedItemWidget::deleteItem, this, &CreateVGWidget::onDeleteItemClicked);

            selectedLayout->addWidget(selectedItemWidget);
        }

        selectedLayout->addStretch();
        selectedLayout->setSpacing(1);
        selectedLayout->setContentsMargins(0, 0, 0, 0);

        QWidget *widget = new QWidget;
        widget->setObjectName("selectedScrollArea");
        widget->setFixedSize(760, m_curSeclectData.count() * 37);
        widget->setLayout(selectedLayout);
        m_selectedScrollArea->setWidget(widget);

        if (isUnallocated) {
            m_selectSpaceStackedWidget->setCurrentIndex(0);
            m_selectSpaceLineEdit->lineEdit()->setPlaceholderText(QString("%1-%2").arg(QString::number(m_minSize, 'f', 2))
                                                                  .arg(QString::number(m_maxSize, 'f', 2)));
            m_selectSpaceLabel->setText(QString("(%1-%2)").arg(QString::number(m_minSize, 'f', 2))
                                        .arg(QString::number(m_maxSize, 'f', 2)) + m_selectSpaceComboBox->currentText());
            m_selectSpaceLineEdit->setText(QString::number(m_maxSize, 'f', 2));
        } else {
            m_selectSpaceStackedWidget->setCurrentIndex(1);
            m_curSize = m_sumSize;
            m_selectedSpaceLabel->setText(tr("Capacity selected: %1").arg(QString::number(m_minSize, 'f', 2)) + m_selectSpaceComboBox->currentText());
        }
    }
}

void CreateVGWidget::onDeleteItemClicked(PVInfoData pvInfoData)
{
    for (int i = 0; i < m_curSeclectData.count(); i++) {
        PVInfoData infoData = m_curSeclectData.at(i);
        if (pvInfoData.m_partitionPath == infoData.m_partitionPath && pvInfoData.m_sectorStart == infoData.m_sectorStart &&
                pvInfoData.m_sectorEnd == infoData.m_sectorEnd) {
            m_curSeclectData.removeAt(i);
            break;
        }
    }

    updateSelectedData();
}

void CreateVGWidget::onCurrentIndexChanged(int index)
{
    SIZE_UNIT type = getCurSizeUnit();
    m_minSize = 0;
    m_maxSize = 0;
    m_sumSize = 0;

    for (int i = 0; i < m_curSeclectData.count(); i++) {
        PVInfoData pvInfoData = m_curSeclectData.at(i);

        if (pvInfoData.m_partitionPath != "unallocated" && pvInfoData.m_disktype != "unrecognized") {
            m_minSize += Utils::sectorToUnit(pvInfoData.m_sectorEnd - pvInfoData.m_sectorStart + 1,
                                           pvInfoData.m_sectorSize, type);
        }

        m_maxSize += Utils::sectorToUnit(pvInfoData.m_sectorEnd - pvInfoData.m_sectorStart + 1,
                                         pvInfoData.m_sectorSize, type);

        m_sumSize += (pvInfoData.m_sectorEnd - pvInfoData.m_sectorStart + 1) * pvInfoData.m_sectorSize;
    }


    m_selectSpaceLineEdit->lineEdit()->setPlaceholderText(QString("%1-%2").arg(QString::number(m_minSize, 'f', 2))
                                                          .arg(QString::number(m_maxSize, 'f', 2)));
    m_selectSpaceLabel->setText(QString("(%1-%2)").arg(QString::number(m_minSize, 'f', 2))
                                .arg(QString::number(m_maxSize, 'f', 2)) + m_selectSpaceComboBox->currentText());

    m_curSize = m_curSize > m_sumSize ? m_sumSize : m_curSize;

    if (!m_selectSpaceLineEdit->text().isEmpty()) {
        switch (index) {
        case 0: {
            m_selectSpaceLineEdit->setText(QString::number(m_curSize / static_cast<double>(MEBIBYTE), 'f', 2));
            break;
        }
        case 1: {
            m_selectSpaceLineEdit->setText(QString::number(m_curSize / static_cast<double>(GIBIBYTE), 'f', 2));
            break;
        }
        case 2: {
            m_selectSpaceLineEdit->setText(QString::number(m_curSize / static_cast<double>(TEBIBYTE), 'f', 2));
            break;
        }
        default:
            break;
        }
    }
}

void CreateVGWidget::onTextChanged(const QString &text)
{
    double curSize = QString::number(text.toDouble(), 'f', 2).toDouble();
    double minSize = QString::number(m_minSize, 'f', 2).toDouble();
    double maxSize = QString::number(m_maxSize, 'f', 2).toDouble();

    if((curSize < minSize) || (curSize > maxSize) || (text.isEmpty())){
        m_selectSpaceLineEdit->setAlert(true);
        m_doneButton->setDisabled(true);
    }else {
        m_selectSpaceLineEdit->setAlert(false);
        m_doneButton->setDisabled(false);
    }

    if (!m_selectSpaceLineEdit->text().isEmpty()) {
        double curSize = QString::number(m_selectSpaceLineEdit->text().toDouble(), 'f', 2).toDouble();
        switch (m_selectSpaceComboBox->currentIndex()) {
        case 0: {
            m_curSize = static_cast<long long>(curSize * MEBIBYTE);
            break;
        }
        case 1: {
            m_curSize = static_cast<long long>(curSize * GIBIBYTE);
            break;
        }
        case 2: {
            m_curSize = static_cast<long long>(curSize * TEBIBYTE);
            break;
        }
        default:
            break;
        }

        if (curSize >= maxSize) {
            m_curSize = m_sumSize;
        }

        m_curSize = m_curSize > m_sumSize ? m_sumSize : m_curSize;
    }
}

void CreateVGWidget::onVGCreateMessage(const QString &vgMessage)
{
    qDebug() << vgMessage;

    if (m_waterLoadingWidget != nullptr) {
        m_waterLoadingWidget->stopTimer();
    }

    close();

}