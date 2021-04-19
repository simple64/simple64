#include <QDir>
#include <QStandardPaths>
#include <QFileDialog>
#include <QFont>
#include <QFontDatabase>
#include <QColorDialog>
#include <QAbstractButton>
#include <QMessageBox>
#include <QCursor>
#include <QRegExpValidator>
#include <QInputDialog>
#include <QDirIterator>

#include "../PluginAPI.h"
#include "../Config.h"
#include "../DebugDump.h"
#include "ui_configDialog.h"
#include "Settings.h"
#include "ConfigDialog.h"
#include "qevent.h"
#include "osal_keys.h"
#include "QtKeyToHID.h"

static
struct
{
	unsigned short width, height;
	const char *description;
} WindowedModes[] = {
	{ 320, 240, "320 x 240" },
	{ 400, 300, "400 x 300" },
	{ 480, 360, "480 x 360" },
	{ 640, 480, "640 x 480" },
	{ 800, 600, "800 x 600" },
	{ 960, 720, "960 x 720" },
	{ 1024, 768, "1024 x 768" },
	{ 1152, 864, "1152 x 864" },
	{ 1280, 960, "1280 x 960" },
	{ 1280, 1024, "1280 x 1024" },
	{ 1440, 1080, "1440 x 1080" },
	{ 1600, 1024, "1600 x 1024" },
	{ 1600, 1200, "1600 x 1200" }
};
static
const unsigned int numWindowedModes = sizeof(WindowedModes) / sizeof(WindowedModes[0]);

static
u32 pow2(u32 dim)
{
	if (dim == 0)
		return 0;

	return (1<<dim);
}

static
u32 powof(u32 dim)
{
	if (dim == 0)
		return 0;

	u32 num = 2;
	u32 i = 1;

	while (num < dim) {
		num <<= 1;
		i++;
	}

	return i;
}

QString ConfigDialog::_hotkeyDescription(quint32 _idx) const
{
	switch (_idx)
	{
	case Config::HotKey::hkTexDump:
		return tr("Toggle textures dump");
	case Config::HotKey::hkHdTexReload:
		return tr("Reload HD textures");
	case Config::HotKey::hkHdTexToggle:
		return tr("Toggle HD textures");
	case Config::HotKey::hkTexCoordBounds:
		return tr("Toggle texcoords bounds");
	case Config::HotKey::hkNativeResTexrects:
		return tr("Toggle 2D texrects in native resolution");
	case Config::HotKey::hkVsync:
		return tr("Toggle VSync");
	case Config::HotKey::hkFBEmulation:
		return tr("Toggle frame buffer emulation");
	case Config::HotKey::hkN64DepthCompare:
		return tr("Toggle N64 depth compare");
	case Config::HotKey::hkOsdVis:
		return tr("Toggle OSD VI/S");
	case Config::HotKey::hkOsdFps:
		return tr("Toggle OSD FPS");
	case Config::HotKey::hkOsdPercent:
		return tr("Toggle OSD percent");
	case Config::HotKey::hkOsdInternalResolution:
		return tr("Toggle OSD internal resolution");
	case Config::HotKey::hkOsdRenderingResolution:
		return tr("Toggle OSD rendering resolution");
	case Config::HotKey::hkForceGammaCorrection:
		return tr("Toggle force gamma correction");
	}
	return tr("Unknown hotkey");
}

class HotkeyItemWidget : public QWidget
{
//	Q_OBJECT
public:
	HotkeyItemWidget(QWidget* pParent = Q_NULLPTR) : QWidget(pParent) {}
	quint32 hidCode() const { return m_hid; }
	void setHidCode(quint32 _code) { m_hid = _code; }
private:
	quint32 m_hid = 0;
};

void ConfigDialog::_init(bool reInit, bool blockCustomSettings)
{
	if (m_blockReInit)
		return;
	m_blockReInit = true;

	if (reInit && m_romName != nullptr && ui->customSettingsCheckBox->isChecked() && ui->settingsDestGameRadioButton->isChecked()) {
		wchar_t strCustomFolderPath[PLUGIN_PATH_SIZE];
		api().FindPluginPath(strCustomFolderPath);
		loadCustomRomSettings(QString::fromWCharArray(strCustomFolderPath), m_romName);
	} else if (reInit) {
		loadSettings(m_strIniPath);
	}

	// Video settings
	QStringList windowedModesList;
	int windowedModesCurrent = -1;
	for (unsigned int i = 0; i < numWindowedModes; ++i) {
		windowedModesList.append(WindowedModes[i].description);
		if (WindowedModes[i].width == config.video.windowedWidth &&
			WindowedModes[i].height == config.video.windowedHeight)
			windowedModesCurrent = i;
	}
	ui->windowedResolutionComboBox->clear();
	ui->windowedResolutionComboBox->insertItems(0, windowedModesList);
	if (windowedModesCurrent > -1)
		ui->windowedResolutionComboBox->setCurrentIndex(windowedModesCurrent);
	else
		ui->windowedResolutionComboBox->setCurrentText(
			QString::number(config.video.windowedWidth) + " x " +
			QString::number(config.video.windowedHeight)
		);

	// matches w x h where w is 300-7999 and h is 200-3999, spaces around x optional
	QRegExp windowedRegExp("([3-9][0-9]{2}|[1-7][0-9]{3}) ?x ?([2-9][0-9]{2}|[1-3][0-9]{3})");
	QValidator *windowedValidator = new QRegExpValidator(windowedRegExp, this);
	ui->windowedResolutionComboBox->setValidator(windowedValidator);

	ui->overscanCheckBox->toggle();
	ui->overscanCheckBox->setChecked(config.frameBufferEmulation.enableOverscan != 0);
	ui->overscanNtscLeftSpinBox->setValue(config.frameBufferEmulation.overscanNTSC.left);
	ui->overscanNtscRightSpinBox->setValue(config.frameBufferEmulation.overscanNTSC.right);
	ui->overscanNtscTopSpinBox->setValue(config.frameBufferEmulation.overscanNTSC.top);
	ui->overscanNtscBottomSpinBox->setValue(config.frameBufferEmulation.overscanNTSC.bottom);
	ui->overscanPalLeftSpinBox->setValue(config.frameBufferEmulation.overscanPAL.left);
	ui->overscanPalRightSpinBox->setValue(config.frameBufferEmulation.overscanPAL.right);
	ui->overscanPalTopSpinBox->setValue(config.frameBufferEmulation.overscanPAL.top);
	ui->overscanPalBottomSpinBox->setValue(config.frameBufferEmulation.overscanPAL.bottom);

	ui->fullScreenCheckBox->setChecked(config.video.fullscreen != 0);

	if (m_maxMSAA == 0 && config.video.maxMultiSampling == 0) {
		// default value
		m_maxMSAA = 8;
	} else if (m_maxMSAA == 0 && config.video.maxMultiSampling != 0) {
		// use cached value
		m_maxMSAA = config.video.maxMultiSampling;
	} else {
		// assign cached value
		config.video.maxMultiSampling = m_maxMSAA;
	}

	const unsigned int multisampling = config.video.fxaa == 0 && config.video.multisampling > 0
		? std::min(config.video.multisampling, m_maxMSAA)
		: m_maxMSAA;

	ui->aliasingSlider->blockSignals(true);
	ui->aliasingSlider->setMaximum(powof(m_maxMSAA));
	ui->aliasingSlider->setValue(powof(multisampling));
	ui->aliasingSlider->blockSignals(false);
	ui->aliasingLabelVal->setText(QString::number(multisampling));
	ui->anisotropicSlider->setValue(config.texture.maxAnisotropy);
	ui->vSyncCheckBox->setChecked(config.video.verticalSync != 0);
	ui->threadedVideoCheckBox->setChecked(config.video.threadedVideo != 0);

	switch (config.texture.bilinearMode) {
	case BILINEAR_3POINT:
		ui->blnr3PointRadioButton->setChecked(true);
		break;
	case BILINEAR_STANDARD:
		ui->blnrStandardRadioButton->setChecked(true);
		break;
	}

	ui->ditheringModeComboBox->setCurrentIndex(config.generalEmulation.rdramImageDitheringMode);
	ui->ditheringQuantizationCheckBox->setChecked(config.generalEmulation.enableDitheringQuantization);
	ui->hiresNoiseDitheringCheckBox->setChecked(config.generalEmulation.enableHiresNoiseDithering);
	ui->ditheringPatternCheckBox->setChecked(config.generalEmulation.enableDitheringPattern);

	switch (config.texture.screenShotFormat) {
	case 0:
		ui->pngRadioButton->setChecked(true);
		break;
	case 1:
		ui->jpegRadioButton->setChecked(true);
		break;
	}

	// Emulation settings
	ui->emulateLodCheckBox->setChecked(config.generalEmulation.enableLOD != 0);
	ui->enableHWLightingCheckBox->setChecked(config.generalEmulation.enableHWLighting != 0);
	ui->enableCoverageCheckBox->setChecked(config.generalEmulation.enableCoverage != 0);
	ui->enableShadersStorageCheckBox->setChecked(config.generalEmulation.enableShadersStorage != 0);
	if (!blockCustomSettings)
		ui->customSettingsCheckBox->setChecked(config.generalEmulation.enableCustomSettings != 0);

	switch (config.graphics2D.correctTexrectCoords) {
	case Config::tcDisable:
		ui->fixTexrectDisableRadioButton->setChecked(true);
		break;
	case Config::tcSmart:
		ui->fixTexrectSmartRadioButton->setChecked(true);
		break;
	case Config::tcForce:
		ui->fixTexrectForceRadioButton->setChecked(true);
		break;
	}

	switch (config.graphics2D.bgMode) {
	case Config::BGMode::bgOnePiece:
		ui->bgModeOnePieceRadioButton->setChecked(true);
		break;
	case Config::BGMode::bgStripped:
		ui->bgModeStrippedRadioButton->setChecked(true);
		break;
	}

	ui->halosRemovalCheckBox->setChecked(config.texture.enableHalosRemoval != 0);
	ui->texCoordBoundsCheckBox->setChecked(config.graphics2D.enableTexCoordBounds != 0);

	ui->nativeRes2DComboBox->setCurrentIndex(config.graphics2D.enableNativeResTexrects);

	ui->gammaCorrectionNoteFrame->setHidden(true);
	ui->gammaLevelSpinBox->setValue(config.gammaCorrection.force != 0 ? config.gammaCorrection.level : 2.0);
	ui->gammaCorrectionCheckBox->setChecked(config.gammaCorrection.force != 0);

	ui->frameBufferSwapComboBox->setCurrentIndex(config.frameBufferEmulation.bufferSwapMode);

	ui->fbInfoEnableCheckBox->toggle();
	ui->fbInfoEnableCheckBox->setChecked(config.frameBufferEmulation.fbInfoDisabled == 0);

	ui->frameBufferCheckBox->toggle();
	const bool fbEmulationEnabled = config.frameBufferEmulation.enable != 0;
	ui->frameBufferCheckBox->setChecked(fbEmulationEnabled);
	ui->frameBufferInfoFrame->setVisible(!fbEmulationEnabled);
	ui->frameBufferInfoFrame2->setVisible(!fbEmulationEnabled);

	ui->copyColorBufferComboBox->setCurrentIndex(config.frameBufferEmulation.copyToRDRAM);
	ui->copyDepthBufferComboBox->setCurrentIndex(config.frameBufferEmulation.copyDepthToRDRAM);
	ui->RenderFBCheckBox->setChecked(config.frameBufferEmulation.copyFromRDRAM != 0);
	ui->copyDepthToMainDepthBufferCheckBox->setChecked(config.frameBufferEmulation.copyDepthToMainDepthBuffer != 0);
	ui->n64DepthCompareComboBox->setCurrentIndex(config.frameBufferEmulation.N64DepthCompare);
	on_n64DepthCompareComboBox_currentIndexChanged(config.frameBufferEmulation.N64DepthCompare);
	ui->forceDepthBufferClearCheckBox->setChecked(config.frameBufferEmulation.forceDepthBufferClear != 0);

	if (config.video.fxaa != 0)
		ui->fxaaRadioButton->setChecked(true);
	else if (config.video.multisampling == 0)
		ui->noaaRadioButton->setChecked(true);
	else
		ui->msaaRadioButton->setChecked(true);

	switch (config.frameBufferEmulation.aspect) {
	case Config::aStretch:
		ui->aspectComboBox->setCurrentIndex(2);
		break;
	case Config::a43:
		ui->aspectComboBox->setCurrentIndex(0);
		break;
	case Config::a169:
		ui->aspectComboBox->setCurrentIndex(1);
		break;
	case Config::aAdjust:
		ui->aspectComboBox->setCurrentIndex(3);
		break;
	}

	ui->resolutionFactorSpinBox->valueChanged(2);
	ui->factor0xRadioButton->toggle();
	ui->factor1xRadioButton->toggle();
	ui->factorXxRadioButton->toggle();
	switch (config.frameBufferEmulation.nativeResFactor) {
	case 0:
		ui->factor0xRadioButton->setChecked(true);
		break;
	case 1:
		ui->factor1xRadioButton->setChecked(true);
		break;
	default:
		ui->factorXxRadioButton->setChecked(true);
		ui->resolutionFactorSpinBox->setValue(config.frameBufferEmulation.nativeResFactor);
		break;
	}

	ui->copyAuxBuffersCheckBox->setChecked(config.frameBufferEmulation.copyAuxToRDRAM != 0);

	ui->readColorChunkCheckBox->setChecked(config.frameBufferEmulation.fbInfoReadColorChunk != 0);
	ui->readColorChunkCheckBox->setEnabled(fbEmulationEnabled && config.frameBufferEmulation.fbInfoDisabled == 0);
	ui->readDepthChunkCheckBox->setChecked(config.frameBufferEmulation.fbInfoReadDepthChunk != 0);
	ui->readDepthChunkCheckBox->setEnabled(fbEmulationEnabled && config.frameBufferEmulation.fbInfoDisabled == 0);

#if 0
	// Texture filter settings
	ui->filterComboBox->setCurrentIndex(config.textureFilter.txFilterMode);
	ui->enhancementComboBox->setCurrentIndex(config.textureFilter.txEnhancementMode);

	ui->textureFilterCacheSpinBox->setValue(config.textureFilter.txCacheSize / gc_uMegabyte);
	ui->deposterizeCheckBox->setChecked(config.textureFilter.txDeposterize != 0);
	ui->ignoreBackgroundsCheckBox->setChecked(config.textureFilter.txFilterIgnoreBG != 0);

	ui->texturePackGroupBox->setChecked(config.textureFilter.txHiresEnable != 0);
	ui->alphaChannelCheckBox->setChecked(config.textureFilter.txHiresFullAlphaChannel != 0);
	ui->alternativeCRCCheckBox->setChecked(config.textureFilter.txHresAltCRC != 0);
	ui->force16bppCheckBox->setChecked(config.textureFilter.txForce16bpp != 0);
	ui->compressCacheCheckBox->setChecked(config.textureFilter.txCacheCompression != 0);
	ui->saveTextureCacheCheckBox->setChecked(config.textureFilter.txSaveCache != 0);
	ui->enhancedTexFileStorageCheckBox->setChecked(config.textureFilter.txEnhancedTextureFileStorage != 0);
	ui->hiresTexFileStorageCheckBox->setChecked(config.textureFilter.txHiresTextureFileStorage != 0);

	ui->texPackPathLineEdit->setText(QString::fromWCharArray(config.textureFilter.txPath));
	ui->texCachePathLineEdit->setText(QString::fromWCharArray(config.textureFilter.txCachePath));
	ui->texDumpPathLineEdit->setText(QString::fromWCharArray(config.textureFilter.txDumpPath));

#endif
	// OSD settings
	QString fontName(config.font.name.c_str());
	ui->fontLineEdit->setText(fontName);
	m_font = QFont(fontName.left(fontName.indexOf(".ttf")));
	m_font.setPixelSize(config.font.size);

	ui->fontLineEdit->setHidden(true);

	ui->fontSizeSpinBox->setValue(config.font.size);

	m_color = QColor(config.font.color[0], config.font.color[1], config.font.color[2]);
	QPalette palette;
	palette.setColor(QPalette::WindowText, m_color);
	palette.setColor(QPalette::Window, Qt::black);
	ui->fontPreviewLabel->setAutoFillBackground(true);
	ui->fontPreviewLabel->setPalette(palette);
	ui->PickFontColorButton->setStyleSheet(QString("color:") + m_color.name());

	switch (config.onScreenDisplay.pos) {
	case Config::posTopLeft:
		ui->topLeftPushButton->setChecked(true);
		break;
	case Config::posTopCenter:
		ui->topPushButton->setChecked(true);
		break;
	case Config::posTopRight:
		ui->topRightPushButton->setChecked(true);
		break;
	case Config::posBottomLeft:
		ui->bottomLeftPushButton->setChecked(true);
		break;
	case Config::posBottomCenter:
		ui->bottomPushButton->setChecked(true);
		break;
	case Config::posBottomRight:
		ui->bottomRightPushButton->setChecked(true);
		break;
	}

	ui->fpsCheckBox->setChecked(config.onScreenDisplay.fps != 0);
	ui->visCheckBox->setChecked(config.onScreenDisplay.vis != 0);
	ui->percentCheckBox->setChecked(config.onScreenDisplay.percent != 0);
	ui->internalResolutionCheckBox->setChecked(config.onScreenDisplay.internalResolution != 0);
	ui->renderingResolutionCheckBox->setChecked(config.onScreenDisplay.renderingResolution != 0);
	ui->statisticsCheckBox->setChecked(config.onScreenDisplay.statistics != 0);

	// Buttons
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Save and Close"));
	ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("Save"));
	ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("Close"));
	ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->setText(tr("Restore Defaults"));

	ui->dumpLowCheckBox->setChecked((config.debug.dumpMode & DEBUG_LOW) != 0);
	ui->dumpNormalCheckBox->setChecked((config.debug.dumpMode & DEBUG_NORMAL) != 0);
	ui->dumpDetailCheckBox->setChecked((config.debug.dumpMode & DEBUG_DETAIL) != 0);

	{
		ui->hotkeyListWidget->clear();
		for (quint32 idx = 0; idx < Config::HotKey::hkTotal; ++idx) {
			HotkeyItemWidget* pWgt = new HotkeyItemWidget;
			QLayout* pLayout = new QHBoxLayout;
			pLayout->addWidget(new QLabel(_hotkeyDescription(idx)));

			QPushButton* pBtn = new QPushButton(tr("Click me"));
			connect(pBtn, SIGNAL(clicked()), SLOT(on_btn_clicked()));
			pLayout->addWidget(pBtn);
			pWgt->setLayout(pLayout);

			QListWidgetItem* pItem = new QListWidgetItem(ui->hotkeyListWidget);
			pItem->setSizeHint(pWgt->sizeHint());
			pItem->setCheckState(Qt::Unchecked);
			ui->hotkeyListWidget->setItemWidget(pItem, pWgt);

			if (config.hotkeys.keys[idx] != 0) {
				pWgt->setHidCode(config.hotkeys.keys[idx]);
				pBtn->setText(osal_keycode_name(config.hotkeys.keys[idx]));
				pItem->setCheckState(Qt::Checked);
			}
		}
	}

#ifndef DEBUG_DUMP
	for (int i = 0; i < ui->tabWidget->count(); ++i) {
		if (tr("Debug") == ui->tabWidget->tabText(i)) {
			ui->tabWidget->removeTab(i);
			break;
		}
	}
#endif

	m_blockReInit = false;
}

void ConfigDialog::_getTranslations(QStringList & _translationFiles) const
{
	QDir pluginFolder(m_strIniPath);
	QStringList nameFilters("gliden64_*.qm");
	_translationFiles = pluginFolder.entryList(nameFilters, QDir::Files, QDir::Name);
}

void ConfigDialog::setIniPath(const QString & _strIniPath)
{
	m_strIniPath = _strIniPath;

	QStringList translationFiles;
	_getTranslations(translationFiles);

	const QString currentTranslation = getTranslationFile();
	int listIndex = 0;
	QStringList translationLanguages("English");
	for (int i = 0; i < translationFiles.size(); ++i) {
		// get locale extracted by filename
		QString locale = translationFiles[i]; // "TranslationExample_de.qm"
		const bool bCurrent = locale == currentTranslation;
		locale.truncate(locale.lastIndexOf('.')); // "TranslationExample_de"
		locale.remove(0, locale.indexOf('_') + 1); // "de"
		QString language = QLocale(locale).nativeLanguageName();
		QString firstChar = language.left(1).toUpper();
		language = firstChar + language.remove(0, 1);
		if (bCurrent) {
			listIndex = i + 1;
		}
		translationLanguages << language;
	}

	ui->translationsComboBox->insertItems(0, translationLanguages);
	ui->translationsComboBox->setCurrentIndex(listIndex);

	// Profile
	ui->profilesComboBox->blockSignals(true);
	const QStringList aProfiles = getProfiles(m_strIniPath);
	ui->profilesComboBox->addItems(aProfiles);
	ui->profilesComboBox->setCurrentIndex(aProfiles.indexOf(getCurrentProfile(m_strIniPath)));
	ui->profilesComboBox->insertSeparator(ui->profilesComboBox->count());
	ui->profilesComboBox->insertItem(ui->profilesComboBox->count(), tr("New..."));
	ui->profilesComboBox->blockSignals(false);
	ui->removeProfilePushButton->setEnabled(ui->profilesComboBox->count() > 3);
}

void ConfigDialog::setRomName(const char * _romName)
{
	const bool bRomNameIsEmpty = _romName == nullptr || strlen(_romName) == 0;
	m_romName = bRomNameIsEmpty ? nullptr : _romName;
	this->on_customSettingsCheckBox_toggled(ui->customSettingsCheckBox->isChecked());
}

ConfigDialog::ConfigDialog(QWidget *parent, Qt::WindowFlags f, unsigned int _maxMsaaLevel) :
QDialog(parent, f),
ui(new Ui::ConfigDialog),
m_maxMSAA(_maxMsaaLevel),
m_accepted(false),
m_fontsInited(false),
m_blockReInit(false)
{
	ui->setupUi(this);
	_init();
}

ConfigDialog::~ConfigDialog()
{
	delete ui;
}

void ConfigDialog::accept(bool justSave) {

	m_accepted = true;

	int windowedValidatorPos = 0;
	QString currentText = ui->windowedResolutionComboBox->currentText();
	if (ui->windowedResolutionComboBox->validator()->validate(
		currentText, windowedValidatorPos
	) == QValidator::Acceptable) {
		QStringList windowedResolutionDimensions = currentText.split("x");
		config.video.windowedWidth = windowedResolutionDimensions[0].trimmed().toInt();
		config.video.windowedHeight = windowedResolutionDimensions[1].trimmed().toInt();
	}

	config.video.fullscreen = ui->fullScreenCheckBox->isChecked() ? 1 : 0;

	config.video.fxaa = ui->fxaaRadioButton->isChecked() ? 1 : 0;
	config.video.multisampling =
		(ui->fxaaRadioButton->isChecked()
			|| ui->n64DepthCompareComboBox->currentIndex() != 0
			|| ui->noaaRadioButton->isChecked()
		) ? 0
		: pow2(ui->aliasingSlider->value());
	config.texture.maxAnisotropy = ui->anisotropicSlider->value();

	if (ui->blnrStandardRadioButton->isChecked())
		config.texture.bilinearMode = BILINEAR_STANDARD;
	else if (ui->blnr3PointRadioButton->isChecked())
		config.texture.bilinearMode = BILINEAR_3POINT;

	config.generalEmulation.rdramImageDitheringMode = ui->ditheringModeComboBox->currentIndex();
	config.generalEmulation.enableDitheringQuantization = ui->ditheringQuantizationCheckBox->isChecked() ? 1 : 0;
	config.generalEmulation.enableHiresNoiseDithering = ui->hiresNoiseDitheringCheckBox->isChecked() ? 1 : 0;
	config.generalEmulation.enableDitheringPattern = ui->ditheringPatternCheckBox->isChecked() ? 1 : 0;

	if (ui->pngRadioButton->isChecked())
		config.texture.screenShotFormat = 0;
	else if (ui->jpegRadioButton->isChecked())
		config.texture.screenShotFormat = 1;

	const int lanuageIndex = ui->translationsComboBox->currentIndex();
	if (lanuageIndex == 0) // English
		config.translationFile.clear();
	else {
		QStringList translationFiles;
		_getTranslations(translationFiles);
		config.translationFile = translationFiles[lanuageIndex-1].toLocal8Bit().constData();
	}

	config.video.verticalSync = ui->vSyncCheckBox->isChecked() ? 1 : 0;
	config.video.threadedVideo = ui->threadedVideoCheckBox->isChecked() ? 1 : 0;

	// Emulation settings
	config.generalEmulation.enableLOD = ui->emulateLodCheckBox->isChecked() ? 1 : 0;
	config.generalEmulation.enableHWLighting = ui->enableHWLightingCheckBox->isChecked() ? 1 : 0;
	config.generalEmulation.enableCoverage = ui->enableCoverageCheckBox->isChecked() ? 1 : 0;
	config.generalEmulation.enableShadersStorage = ui->enableShadersStorageCheckBox->isChecked() ? 1 : 0;
	config.generalEmulation.enableCustomSettings = ui->customSettingsCheckBox->isChecked() ? 1 : 0;

	config.gammaCorrection.force = ui->gammaCorrectionCheckBox->isChecked() ? 1 : 0;
	config.gammaCorrection.level = ui->gammaLevelSpinBox->value();

	if (ui->fixTexrectDisableRadioButton->isChecked())
		config.graphics2D.correctTexrectCoords = Config::tcDisable;
	else if (ui->fixTexrectSmartRadioButton->isChecked())
		config.graphics2D.correctTexrectCoords = Config::tcSmart;
	else if (ui->fixTexrectForceRadioButton->isChecked())
		config.graphics2D.correctTexrectCoords = Config::tcForce;

	if (ui->bgModeOnePieceRadioButton->isChecked())
		config.graphics2D.bgMode = Config::BGMode::bgOnePiece;
	else if (ui->bgModeStrippedRadioButton->isChecked())
		config.graphics2D.bgMode = Config::BGMode::bgStripped;

	config.texture.enableHalosRemoval = ui->halosRemovalCheckBox->isChecked() ? 1 : 0;
	config.graphics2D.enableTexCoordBounds = ui->texCoordBoundsCheckBox->isChecked() ? 1 : 0;
	config.graphics2D.enableNativeResTexrects = ui->nativeRes2DComboBox->currentIndex();

	config.frameBufferEmulation.enable = ui->frameBufferCheckBox->isChecked() ? 1 : 0;

	config.frameBufferEmulation.bufferSwapMode = ui->frameBufferSwapComboBox->currentIndex();
	config.frameBufferEmulation.copyToRDRAM = ui->copyColorBufferComboBox->currentIndex();
	config.frameBufferEmulation.copyDepthToRDRAM = ui->copyDepthBufferComboBox->currentIndex();
	config.frameBufferEmulation.copyFromRDRAM = ui->RenderFBCheckBox->isChecked() ? 1 : 0;
	config.frameBufferEmulation.copyDepthToMainDepthBuffer = ui->copyDepthToMainDepthBufferCheckBox->isChecked() ? 1 : 0;

	config.frameBufferEmulation.N64DepthCompare = ui->n64DepthCompareComboBox->currentIndex();
	config.frameBufferEmulation.forceDepthBufferClear = ui->forceDepthBufferClearCheckBox->isChecked() ? 1 : 0;

	if (ui->aspectComboBox->currentIndex() == 2)
		config.frameBufferEmulation.aspect = Config::aStretch;
	else if (ui->aspectComboBox->currentIndex() == 0)
		config.frameBufferEmulation.aspect = Config::a43;
	else if (ui->aspectComboBox->currentIndex() == 1)
		config.frameBufferEmulation.aspect = Config::a169;
	else if (ui->aspectComboBox->currentIndex() == 3)
		config.frameBufferEmulation.aspect = Config::aAdjust;

	if (ui->factor0xRadioButton->isChecked())
		config.frameBufferEmulation.nativeResFactor = 0;
	else if (ui->factor1xRadioButton->isChecked())
		config.frameBufferEmulation.nativeResFactor = 1;
	else if (ui->factorXxRadioButton->isChecked())
		config.frameBufferEmulation.nativeResFactor = ui->resolutionFactorSpinBox->value();

	config.frameBufferEmulation.copyAuxToRDRAM = ui->copyAuxBuffersCheckBox->isChecked() ? 1 : 0;
	config.frameBufferEmulation.fbInfoDisabled = ui->fbInfoEnableCheckBox->isChecked() ? 0 : 1;
	config.frameBufferEmulation.fbInfoReadColorChunk = ui->readColorChunkCheckBox->isChecked() ? 1 : 0;
	config.frameBufferEmulation.fbInfoReadDepthChunk = ui->readDepthChunkCheckBox->isChecked() ? 1 : 0;

	config.frameBufferEmulation.enableOverscan = ui->overscanCheckBox->isChecked() ? 1 : 0;
	config.frameBufferEmulation.overscanNTSC.left = ui->overscanNtscLeftSpinBox->value();
	config.frameBufferEmulation.overscanNTSC.right = ui->overscanNtscRightSpinBox->value();
	config.frameBufferEmulation.overscanNTSC.top = ui->overscanNtscTopSpinBox->value();
	config.frameBufferEmulation.overscanNTSC.bottom = ui->overscanNtscBottomSpinBox->value();
	config.frameBufferEmulation.overscanPAL.left = ui->overscanPalLeftSpinBox->value();
	config.frameBufferEmulation.overscanPAL.right = ui->overscanPalRightSpinBox->value();
	config.frameBufferEmulation.overscanPAL.top = ui->overscanPalTopSpinBox->value();
	config.frameBufferEmulation.overscanPAL.bottom = ui->overscanPalBottomSpinBox->value();

#if 0
	// Texture filter settings
	config.textureFilter.txFilterMode = ui->filterComboBox->currentIndex();
	config.textureFilter.txEnhancementMode = ui->enhancementComboBox->currentIndex();

	config.textureFilter.txCacheSize = ui->textureFilterCacheSpinBox->value() * gc_uMegabyte;
	config.textureFilter.txDeposterize = ui->deposterizeCheckBox->isChecked() ? 1 : 0;
	config.textureFilter.txFilterIgnoreBG = ui->ignoreBackgroundsCheckBox->isChecked() ? 1 : 0;

	config.textureFilter.txHiresEnable = ui->texturePackGroupBox->isChecked() ? 1 : 0;
	config.textureFilter.txHiresFullAlphaChannel = ui->alphaChannelCheckBox->isChecked() ? 1 : 0;
	config.textureFilter.txHresAltCRC = ui->alternativeCRCCheckBox->isChecked() ? 1 : 0;

	config.textureFilter.txCacheCompression = ui->compressCacheCheckBox->isChecked() ? 1 : 0;
	config.textureFilter.txForce16bpp = ui->force16bppCheckBox->isChecked() ? 1 : 0;
	config.textureFilter.txSaveCache = ui->saveTextureCacheCheckBox->isChecked() ? 1 : 0;
	config.textureFilter.txEnhancedTextureFileStorage = ui->enhancedTexFileStorageCheckBox->isChecked() ? 1 : 0;
	config.textureFilter.txHiresTextureFileStorage = ui->hiresTexFileStorageCheckBox->isChecked() ? 1 : 0;

	QDir txPath(ui->texPackPathLineEdit->text());
	if (!txPath.exists() &&
		!txPath.mkdir(txPath.absolutePath()) &&
		config.textureFilter.txHiresEnable != 0) {
		QMessageBox msgBox;
		msgBox.setStandardButtons(QMessageBox::Close);
		msgBox.setWindowTitle("GLideN64");
		msgBox.setText(tr("Failed to create the texture pack folder. Please change the folder or turn off texture packs."));
		msgBox.exec();
		ui->tabWidget->setCurrentIndex(3);
		ui->texPackPathLineEdit->setFocus(Qt::PopupFocusReason);
		ui->texPackPathLineEdit->selectAll();
		return;
	}
	config.textureFilter.txPath[txPath.path().toWCharArray(config.textureFilter.txPath)] = L'\0';

	QDir txCachePath(ui->texCachePathLineEdit->text());
	if (!txCachePath.exists() &&
		!txCachePath.mkdir(txCachePath.absolutePath()) &&
		config.textureFilter.txHiresEnable != 0) {
		QMessageBox msgBox;
		msgBox.setStandardButtons(QMessageBox::Close);
		msgBox.setWindowTitle("GLideN64");
		msgBox.setText(tr("Failed to create the texture pack cache folder. Please change the folder or turn off texture packs."));
		msgBox.exec();
		ui->tabWidget->setCurrentIndex(3);
		ui->texCachePathLineEdit->setFocus(Qt::PopupFocusReason);
		ui->texCachePathLineEdit->selectAll();
		return;
	}
	config.textureFilter.txCachePath[txCachePath.path().toWCharArray(config.textureFilter.txCachePath)] = L'\0';

	QDir txDumpPath(ui->texDumpPathLineEdit->text());
	if (!txDumpPath.exists() &&
		!txDumpPath.mkdir(txDumpPath.absolutePath()) &&
		config.textureFilter.txHiresEnable != 0 &&
		config.hotkeys.keys[Config::HotKey::hkTexDump] != 0) {
		QMessageBox msgBox;
		msgBox.setStandardButtons(QMessageBox::Close);
		msgBox.setWindowTitle("GLideN64");
		msgBox.setText(tr("Failed to create the texture dump folder. Please change the folder or turn off dumping texture packs."));
		msgBox.exec();
		ui->tabWidget->setCurrentIndex(3);
		ui->texDumpPathLineEdit->setFocus(Qt::PopupFocusReason);
		ui->texDumpPathLineEdit->selectAll();
		return;
	}
	config.textureFilter.txDumpPath[txDumpPath.path().toWCharArray(config.textureFilter.txDumpPath)] = L'\0';

#endif
	// OSD settings
	config.font.size = ui->fontSizeSpinBox->value();
#ifdef OS_WINDOWS
	config.font.name = ui->fontLineEdit->text().toLocal8Bit().constData();
#else
	config.font.name = ui->fontLineEdit->text().toStdString();
#endif
	config.font.color[0] = m_color.red();
	config.font.color[1] = m_color.green();
	config.font.color[2] = m_color.blue();
	config.font.color[3] = m_color.alpha();
	config.font.colorf[0] = m_color.redF();
	config.font.colorf[1] = m_color.greenF();
	config.font.colorf[2] = m_color.blueF();
	config.font.colorf[3] = m_color.alphaF();


	if (ui->topLeftPushButton->isChecked())
		config.onScreenDisplay.pos = Config::posTopLeft;
	else if (ui->topPushButton->isChecked())
		config.onScreenDisplay.pos = Config::posTopCenter;
	else if (ui->topRightPushButton->isChecked())
		config.onScreenDisplay.pos = Config::posTopRight;
	else if (ui->bottomLeftPushButton->isChecked())
		config.onScreenDisplay.pos = Config::posBottomLeft;
	else if (ui->bottomPushButton->isChecked())
		config.onScreenDisplay.pos = Config::posBottomCenter;
	else if (ui->bottomRightPushButton->isChecked())
		config.onScreenDisplay.pos = Config::posBottomRight;

	config.onScreenDisplay.fps = ui->fpsCheckBox->isChecked() ? 1 : 0;
	config.onScreenDisplay.vis = ui->visCheckBox->isChecked() ? 1 : 0;
	config.onScreenDisplay.percent = ui->percentCheckBox->isChecked() ? 1 : 0;
	config.onScreenDisplay.internalResolution = ui->internalResolutionCheckBox->isChecked() ? 1 : 0;
	config.onScreenDisplay.renderingResolution = ui->renderingResolutionCheckBox->isChecked() ? 1 : 0;
	config.onScreenDisplay.statistics = ui->statisticsCheckBox->isChecked() ? 1 : 0;

	for (quint32 idx = 0; idx < Config::HotKey::hkTotal; ++idx) {
		config.hotkeys.keys[idx] = 0;
		QListWidgetItem * pItem = ui->hotkeyListWidget->item(idx);
		if (pItem->checkState() == Qt::Checked) {
			HotkeyItemWidget* pWgt = (HotkeyItemWidget*)ui->hotkeyListWidget->itemWidget(pItem);
			config.hotkeys.keys[idx] = pWgt->hidCode();
		}
	}

	config.debug.dumpMode = 0;
	if (ui->dumpLowCheckBox->isChecked())
		config.debug.dumpMode |= DEBUG_LOW;
	if (ui->dumpNormalCheckBox->isChecked())
		config.debug.dumpMode |= DEBUG_NORMAL;
	if (ui->dumpDetailCheckBox->isChecked())
		config.debug.dumpMode |= DEBUG_DETAIL;

	if (config.generalEmulation.enableCustomSettings && ui->settingsDestGameRadioButton->isChecked() && m_romName != nullptr)
	{
		wchar_t strCustomFolderPath[PLUGIN_PATH_SIZE];
		api().FindPluginPath(strCustomFolderPath);
		saveCustomRomSettings(QString::fromWCharArray(strCustomFolderPath), m_romName);
	}
	else
		writeSettings(m_strIniPath);

	if (!justSave) {
		QDialog::accept();
	}
}

void ConfigDialog::on_PickFontColorButton_clicked()
{
	const QColor color = QColorDialog::getColor(m_color, this);

	if (!color.isValid())
		return;

	m_color = color;
	QPalette palette;
	palette.setColor(QPalette::WindowText, m_color);
	palette.setColor(QPalette::Window, Qt::black);
	ui->fontPreviewLabel->setAutoFillBackground(true);
	ui->fontPreviewLabel->setPalette(palette);
	ui->PickFontColorButton->setStyleSheet(QString("color:") + m_color.name());
}

void ConfigDialog::on_aliasingSlider_valueChanged(int value)
{
	ui->aliasingLabelVal->setText(QString::number(pow2(value)));
	if (value != 0) {
		ui->msaaRadioButton->setChecked(true);
	} else {
		ui->noaaRadioButton->setChecked(true);
	}
}

void ConfigDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	if ((QPushButton *)button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
		QMessageBox msgBox(QMessageBox::Warning, tr("Restore Defaults"),
			tr("Are you sure you want to reset all settings to default?"),
			QMessageBox::RestoreDefaults | QMessageBox::Cancel, this
			);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		msgBox.setButtonText(QMessageBox::RestoreDefaults, tr("Restore Defaults"));
		msgBox.setButtonText(QMessageBox::Cancel, tr("Cancel"));
		if (msgBox.exec() == QMessageBox::RestoreDefaults) {
			const u32 enableCustomSettings = config.generalEmulation.enableCustomSettings;
			config.resetToDefaults();
			config.generalEmulation.enableCustomSettings = enableCustomSettings;
			setTitle();
			setRomName(m_romName);
			_init();
		}
	} else if ((QPushButton *)button == ui->buttonBox->button(QDialogButtonBox::Save)) {
		this->accept(true);
	} else if ((QPushButton *)button == ui->buttonBox->button(QDialogButtonBox::Ok)) {
		this->accept(false);
	}
}

void ConfigDialog::on_windowedResolutionComboBox_currentIndexChanged(int index)
{
	if (index < numWindowedModes)
		ui->windowedResolutionComboBox->clearFocus();
}

void ConfigDialog::on_windowedResolutionComboBox_currentTextChanged(QString text)
{
	if (text == tr("Custom"))
		ui->windowedResolutionComboBox->setCurrentText("");
}

void ConfigDialog::on_overscanCheckBox_toggled(bool checked)
{
	ui->overscanCheckBox->setText(tr("Overscan") + (checked ? QString(":") : QString("")));
}

void ConfigDialog::on_aliasingWarningLabel_linkActivated(QString link)
{
	if (link == "#n64DepthCompare") {
		ui->tabWidget->setCurrentIndex(2);
		ui->n64DepthCompareComboBox->setStyleSheet("background:yellow");
	}
}

void ConfigDialog::on_frameBufferInfoLabel_linkActivated(QString link)
{
	if (link == "#frameBuffer") {
		ui->tabWidget->setCurrentIndex(2);
		ui->frameBufferCheckBox->setStyleSheet("background:yellow");
	}
}

void ConfigDialog::on_customSettingsCheckBox_toggled(bool checked)
{
	const bool romLoaded = checked && m_romName != nullptr;
	if (romLoaded) {
		ui->settingsDestGameRadioButton->setText(QString::fromLatin1(m_romName));
		ui->settingsDestGameRadioButton->setChecked(true);
	} else {
		ui->settingsDestProfileRadioButton->setChecked(true);
	}

	ui->profilesLabel->setHidden(romLoaded);
	ui->settingsDestFrame->setVisible(romLoaded);
	_init(false, true);
}

void ConfigDialog::on_frameBufferInfoLabel2_linkActivated(QString link)
{
	if (link == "#frameBuffer") {
		ui->tabWidget->setCurrentIndex(2);
		ui->frameBufferCheckBox->setStyleSheet("background:yellow");
	}
}

void ConfigDialog::on_frameBufferCheckBox_toggled(bool checked)
{
	ui->readColorChunkCheckBox->setEnabled(checked && ui->fbInfoEnableCheckBox->isChecked());
	ui->readDepthChunkCheckBox->setEnabled(checked && ui->fbInfoEnableCheckBox->isChecked());

	if (!checked && ui->fxaaRadioButton->isChecked())
		ui->noaaRadioButton->setChecked(true);

	ui->frameBufferCheckBox->setStyleSheet("");
}

void ConfigDialog::on_gammaLevelSpinBox_valueChanged(double /*value*/)
{
	ui->gammaCorrectionCheckBox->setChecked(true);
}

void ConfigDialog::on_gammaCorrectionCheckBox_toggled(bool checked)
{
	if (!checked) {
		ui->gammaLevelSpinBox->setValue(2.0);
		ui->gammaCorrectionCheckBox->setChecked(false);
	}
}

void ConfigDialog::on_resolutionFactorSpinBox_valueChanged(int /*value*/)
{
	ui->factorXxRadioButton->setChecked(true);
}

void ConfigDialog::on_fontTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem * /*previous*/)
{
	if (current->childCount() > 0) {
		ui->fontLineEdit->setText(current->child(0)->text(0));
		m_font.setFamily(current->text(0));
	} else {
		ui->fontLineEdit->setText(current->text(0));
		m_font.setFamily(current->parent()->text(0));
	}
	ui->fontPreviewLabel->setFont(m_font);
}

void ConfigDialog::on_fontSizeSpinBox_valueChanged(int value)
{
	m_font.setPixelSize(value);
	ui->fontPreviewLabel->setFont(m_font);
}

void ConfigDialog::on_tabWidget_currentChanged(int tab)
{
	if (!m_fontsInited && ui->tabWidget->tabText(tab) == tr("OSD")) {
		ui->tabWidget->setCursor(QCursor(Qt::WaitCursor));

		QMap<QString, QStringList> internalFontList;
		QString fontDir = QStandardPaths::locate(QStandardPaths::FontsLocation, QString(), QStandardPaths::LocateDirectory);
		QStringList fontFilter;
		fontFilter << "*.ttf";
		QDirIterator fontIt(fontDir, fontFilter, QDir::Files, QDirIterator::Subdirectories);
		while (fontIt.hasNext()) {
			QString font = fontIt.next();
			int id = QFontDatabase::addApplicationFont(font);
			QString fontListFamily = QFontDatabase::applicationFontFamilies(id).at(0);
			internalFontList[fontListFamily].append(font);
		}

		QMap<QString, QStringList>::const_iterator i;
		for (i = internalFontList.constBegin(); i != internalFontList.constEnd(); ++i) {
			QTreeWidgetItem *fontFamily = new QTreeWidgetItem(ui->fontTreeWidget);
			fontFamily->setText(0, i.key());
			for (int j = 0; j < i.value().size(); ++j) {
				QTreeWidgetItem *fontFile = new QTreeWidgetItem(fontFamily);
				fontFile->setText(0, i.value()[j]);
				if (i.value()[j] == ui->fontLineEdit->text()) {
					fontFamily->setExpanded(true);
					fontFile->setSelected(true);
					ui->fontTreeWidget->scrollToItem(fontFile);
					m_font.setFamily(i.key());
					ui->fontPreviewLabel->setFont(m_font);
				}
			}
		}

		ui->tabWidget->setCursor(QCursor(Qt::ArrowCursor));
		m_fontsInited = true;
	}

	ui->n64DepthCompareComboBox->setStyleSheet("");
	ui->frameBufferCheckBox->setStyleSheet("");
}

void ConfigDialog::setTitle()
{
	setWindowTitle(tr("GLideN64 Settings"));
}

void ConfigDialog::on_profilesComboBox_currentIndexChanged(const QString &profile)
{
	ui->settingsDestProfileRadioButton->setChecked(true);
	if (profile == tr("New...")) {
		bool ok;
		QString switchToProfile = getCurrentProfile(m_strIniPath);
		QString newProfile = QInputDialog::getText(this,
			tr("New Profile"), tr("New profile name:"), QLineEdit::Normal, "", &ok,
			Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
		if (ok) {
			ui->profilesComboBox->blockSignals(true);
			if (newProfile == tr("New...")) {
				QMessageBox msgBox(QMessageBox::Warning, tr("New Profile"),
					tr("New settings profiles cannot be called \"New...\"."),
					QMessageBox::Close, this
				);
				msgBox.exec();
			} else if (newProfile.isEmpty()) {
				QMessageBox msgBox(QMessageBox::Warning, tr("New Profile"),
					tr("Please type a name for your new settings profile."),
					QMessageBox::Close, this
				);
				msgBox.exec();
			} else if (getProfiles(m_strIniPath).contains(newProfile)) {
				QMessageBox msgBox(QMessageBox::Warning, tr("New Profile"),
					tr("This settings profile already exists."),
					QMessageBox::Close, this
				);
				msgBox.exec();
			} else {
				ui->profilesComboBox->insertItem(0, newProfile);
				addProfile(m_strIniPath, newProfile);
				if (ui->profilesComboBox->count() > 3) {
					ui->removeProfilePushButton->setEnabled(true);
				}
				switchToProfile = newProfile;
			}
			ui->profilesComboBox->blockSignals(false);
		}
		for (int i = 0; i < ui->profilesComboBox->count(); ++i) {
			if (ui->profilesComboBox->itemText(i) == switchToProfile) {
				ui->profilesComboBox->setCurrentIndex(i);
				break;
			}
		}
		return;
	}
	changeProfile(m_strIniPath, profile);
	_init(true);
}

void ConfigDialog::on_settingsDestProfileRadioButton_toggled(bool /*checked*/)
{
	_init(true, true);
}

void ConfigDialog::on_removeProfilePushButton_clicked()
{
	if (ui->profilesComboBox->count() == 3)
		return;

	QString profile = ui->profilesComboBox->currentText();
	if (!getProfiles(m_strIniPath).contains(profile))
		return;
	QString msg(tr("Are you sure you want to remove the settings profile \""));
	msg += "" + profile + tr("\"?");
	QMessageBox msgBox(QMessageBox::Warning, tr("Remove Profile"),
		msg, QMessageBox::Yes | QMessageBox::Cancel, this);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	msgBox.setButtonText(QMessageBox::Yes, tr("Remove"));
	msgBox.setButtonText(QMessageBox::No, tr("Cancel"));
	if (msgBox.exec() == QMessageBox::Yes) {
		removeProfile(m_strIniPath, profile);
		ui->profilesComboBox->blockSignals(true);
		ui->profilesComboBox->removeItem(ui->profilesComboBox->currentIndex());
		changeProfile(m_strIniPath, ui->profilesComboBox->itemText(ui->profilesComboBox->currentIndex()));
		ui->profilesComboBox->blockSignals(false);
		_init(true);
		ui->removeProfilePushButton->setDisabled(ui->profilesComboBox->count() == 3);
	}
}

void ConfigDialog::on_nativeRes2DComboBox_currentIndexChanged(int index)
{
	ui->fixTexrectFrame->setEnabled(index == 0);
}

void ConfigDialog::on_n64DepthCompareComboBox_currentIndexChanged(int index)
{
	ui->aliasingWarningFrame->setVisible(index > 0);
	ui->aliasingSliderFrame->setDisabled(index > 0);
	if (index > 0 && ui->msaaRadioButton->isChecked())
		ui->fxaaRadioButton->setChecked(true);
	ui->msaaRadioButton->setDisabled(index > 0);
	ui->n64DepthCompareComboBox->setStyleSheet("");
}

void ConfigDialog::on_hotkeyListWidget_itemClicked(QListWidgetItem *item)
{
    if (item->checkState() == Qt::Unchecked)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
}

class HotkeyMessageBox: public QMessageBox
{
public:
	HotkeyMessageBox(QWidget *parent = Q_NULLPTR) : QMessageBox(parent)
	{
		setWindowTitle("Hotkey");
		setIcon(QMessageBox::Information);
	}

	void keyPressEvent(QKeyEvent * pEvent) override
	{
		switch (pEvent->key())
		{
		case Qt::Key_Escape:
		case Qt::Key_Return:
		case Qt::Key_Enter:
			QMessageBox::keyPressEvent(pEvent);
			return;
		}
		m_Key = pEvent->key();
		QMessageBox::keyPressEvent(pEvent);
		close();
	}

	quint32 m_Key = Qt::Key::Key_unknown;
};

void ConfigDialog::on_btn_clicked() {
	if (QPushButton* pBtn = qobject_cast<QPushButton*>(sender())) {
		if (QLabel* pLabel = pBtn->parent()->findChild< QLabel* >()) {
			HotkeyMessageBox msgBox(this);
			msgBox.setInformativeText(QString("Press a key for ") + pLabel->text());
			msgBox.exec();
			if (msgBox.m_Key != Qt::Key::Key_unknown) {
				const unsigned int hidCode = QtKeyToHID(msgBox.m_Key);
				for (quint32 idx = 0; idx < Config::HotKey::hkTotal; ++idx) {
					QListWidgetItem * pItem = ui->hotkeyListWidget->item(idx);
					HotkeyItemWidget* pWgt = (HotkeyItemWidget*)ui->hotkeyListWidget->itemWidget(pItem);
					if (pWgt->hidCode() == hidCode) {
						QPushButton* pButton = pWgt->findChild<QPushButton*>();
						if (pButton != nullptr)
							pButton->setText(tr("Click me"));
						pWgt->setHidCode(0u);
						break;
					}
				}
				pBtn->setText(osal_keycode_name(hidCode));
				((HotkeyItemWidget*)pBtn->parent())->setHidCode(hidCode);
			}
		}
	}
}
