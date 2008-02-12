/****************************************************************************
** GLWorld meta object code from reading C++ file 'glworld.h'
**
** Created: Sun Jan 27 20:48:32 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "glworld.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#include <qvariant.h>
const char *GLWorld::className() const
{
    return "GLWorld";
}

QMetaObject *GLWorld::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLWorld( "GLWorld", &GLWorld::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLWorld::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLWorld", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLWorld::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLWorld", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLWorld::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"scheduleUpdate", 0, 0 };
    static const QUMethod slot_1 = {"slotUpdateAnimation", 0, 0 };
    static const QUMethod slot_2 = {"slotSetUseOpenGlKeyer", 0, 0 };
    static const QUMethod slot_3 = {"slotSetUseFastShaderKeyer", 0, 0 };
    static const QUMethod slot_4 = {"slotSetSrcBlendFactor", 0, 0 };
    static const QUMethod slot_5 = {"slotSetDstBlendFactor", 0, 0 };
    static const QUMethod slot_6 = {"slotResetBlendFactor", 0, 0 };
    static const QUMethod slot_7 = {"slotSetCompositingMode", 0, 0 };
    static const QUMethod slot_8 = {"slotLightColorAmbientRed", 0, 0 };
    static const QUMethod slot_9 = {"slotLightColorAmbientGreen", 0, 0 };
    static const QUMethod slot_10 = {"slotLightColorAmbientBlue", 0, 0 };
    static const QUMethod slot_11 = {"slotLightColorDiffuseRed", 0, 0 };
    static const QUMethod slot_12 = {"slotLightColorDiffuseGreen", 0, 0 };
    static const QUMethod slot_13 = {"slotLightColorDiffuseBlue", 0, 0 };
    static const QUMethod slot_14 = {"slotLightColorSpecularRed", 0, 0 };
    static const QUMethod slot_15 = {"slotLightColorSpecularGreen", 0, 0 };
    static const QUMethod slot_16 = {"slotLightColorSpecularBlue", 0, 0 };
    static const QUMethod slot_17 = {"slotLightPosition", 0, 0 };
    static const QUParameter param_slot_18[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_18 = {"slotLightColorAmbient", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_19 = {"slotLightColorDiffuse", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_20 = {"slotLightColorSpecular", 1, param_slot_20 };
    static const QUMethod slot_21 = {"slotChooseFramesPerSecond", 0, 0 };
    static const QUMethod slot_22 = {"slotSetShowLights", 0, 0 };
    static const QUMethod slot_23 = {"grabDesktop", 0, 0 };
    static const QUMethod slot_24 = {"paintGL", 0, 0 };
    static const QUParameter param_slot_25[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_25 = {"layerClicked", 1, param_slot_25 };
    static const QUParameter param_slot_26[] = {
	{ "layernumber", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_26 = {"effectNodeClicked", 1, param_slot_26 };
    static const QUParameter param_slot_27[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_27 = {"mousePressEvent", 1, param_slot_27 };
    static const QUParameter param_slot_28[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_28 = {"mouseReleaseEvent", 1, param_slot_28 };
    static const QUParameter param_slot_29[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_29 = {"mouseMoveEvent", 1, param_slot_29 };
    static const QUMethod slot_30 = {"setSelectColorsIndividually", 0, 0 };
    static const QUMethod slot_31 = {"connectKeyframer", 0, 0 };
    static const QUParameter param_slot_32[] = {
	{ "parentwidget", &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_32 = {"addKeyframer", 1, param_slot_32 };
    static const QUParameter param_slot_33[] = {
	{ "renval", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_33 = {"setResolution", 1, param_slot_33 };
    static const QUParameter param_slot_34[] = {
	{ "renval", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_34 = {"updateResMenu", 1, param_slot_34 };
    static const QUParameter param_slot_35[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_35 = {"setXRotation", 1, param_slot_35 };
    static const QUParameter param_slot_36[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_36 = {"setYRotation", 1, param_slot_36 };
    static const QUParameter param_slot_37[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_37 = {"setZRotation", 1, param_slot_37 };
    static const QUParameter param_slot_38[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_38 = {"setScaleValue", 1, param_slot_38 };
    static const QUParameter param_slot_39[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out },
	{ 0, &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_39 = {"getScaleValue", 2, param_slot_39 };
    static const QUParameter param_slot_40[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_40 = {"setXScale", 1, param_slot_40 };
    static const QUParameter param_slot_41[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_41 = {"setYScale", 1, param_slot_41 };
    static const QUParameter param_slot_42[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_42 = {"setZScale", 1, param_slot_42 };
    static const QUParameter param_slot_43[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_43 = {"setXTrans", 1, param_slot_43 };
    static const QUParameter param_slot_44[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_44 = {"setYTrans", 1, param_slot_44 };
    static const QUParameter param_slot_45[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_45 = {"setZTrans", 1, param_slot_45 };
    static const QUParameter param_slot_46[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_46 = {"setAlpha", 1, param_slot_46 };
    static const QUParameter param_slot_47[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_47 = {"setZoom", 1, param_slot_47 };
    static const QUParameter param_slot_48[] = {
	{ "degrees", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_48 = {"setExtrude", 1, param_slot_48 };
    static const QUMethod slot_49 = {"setColor", 0, 0 };
    static const QUMethod slot_50 = {"setKeyframeDrawStatus", 0, 0 };
    static const QUMethod slot_51 = {"slotSetLayerSelectedVisible", 0, 0 };
    static const QUMethod slot_52 = {"resetAllSliders", 0, 0 };
    static const QUParameter param_slot_53[] = {
	{ "x", &static_QUType_double, 0, QUParameter::In },
	{ "y", &static_QUType_double, 0, QUParameter::In },
	{ "z", &static_QUType_double, 0, QUParameter::In },
	{ "a", &static_QUType_double, 0, QUParameter::In },
	{ "b", &static_QUType_double, 0, QUParameter::In },
	{ "c", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_53 = {"spaceEvent", 6, param_slot_53 };
    static const QUMethod slot_54 = {"loadObjObject", 0, 0 };
    static const QUParameter param_slot_55[] = {
	{ "frame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_55 = {"slotSetSlipFrames", 1, param_slot_55 };
    static const QUParameter param_slot_56[] = {
	{ "frame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_56 = {"setinFrames", 1, param_slot_56 };
    static const QUParameter param_slot_57[] = {
	{ "frame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_57 = {"setoutFrames", 1, param_slot_57 };
    static const QUParameter param_slot_58[] = {
	{ "frame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_58 = {"slotSetKeySlipFrames", 1, param_slot_58 };
    static const QUParameter param_slot_59[] = {
	{ "frame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_59 = {"setinKeyFrames", 1, param_slot_59 };
    static const QUParameter param_slot_60[] = {
	{ "frame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_60 = {"setoutKeyFrames", 1, param_slot_60 };
    static const QUMethod slot_61 = {"noTool", 0, 0 };
    static const QUMethod slot_62 = {"toolTranslate", 0, 0 };
    static const QUMethod slot_63 = {"toolScale", 0, 0 };
    static const QUMethod slot_64 = {"toolRotate", 0, 0 };
    static const QUParameter param_slot_65[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_65 = {"changeZoom", 1, param_slot_65 };
    static const QUMethod slot_66 = {"resetZoom", 0, 0 };
    static const QUParameter param_slot_67[] = {
	{ "value", &static_QUType_ptr, "GLfloat", QUParameter::In }
    };
    static const QUMethod slot_67 = {"setZoom", 1, param_slot_67 };
    static const QUParameter param_slot_68[] = {
	{ "hf", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_68 = {"setHFog", 1, param_slot_68 };
    static const QUParameter param_slot_69[] = {
	{ "vf", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_69 = {"setVFog", 1, param_slot_69 };
    static const QUMethod slot_70 = {"updateFog", 0, 0 };
    static const QUMethod slot_71 = {"SetGPUSelect", 0, 0 };
    static const QUMethod slot_72 = {"updateToolDisplay", 0, 0 };
    static const QUMethod slot_73 = {"setKeyStatus", 0, 0 };
    static const QUMethod slot_74 = {"setClipStatus", 0, 0 };
    static const QUMethod slot_75 = {"toggleForegroundStatus", 0, 0 };
    static const QUMethod slot_76 = {"toggleDepthStatus", 0, 0 };
    static const QUParameter param_slot_77[] = {
	{ "text_layer", &static_QUType_ptr, "JahLayer", QUParameter::In },
	{ "text_string", &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_77 = {"setTextLayerText", 2, param_slot_77 };
    static const QUParameter param_slot_78[] = {
	{ "text_string", &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_78 = {"setText", 1, param_slot_78 };
    static const QUParameter param_slot_79[] = {
	{ "q_string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_79 = {"setText", 1, param_slot_79 };
    static const QUMethod slot_80 = {"setFontColor", 0, 0 };
    static const QUMethod slot_81 = {"loadFont", 0, 0 };
    static const QUMethod slot_82 = {"setTextSafe", 0, 0 };
    static const QUMethod slot_83 = {"setGrid", 0, 0 };
    static const QUMethod slot_84 = {"setSmooth", 0, 0 };
    static const QUMethod slot_85 = {"setBlur", 0, 0 };
    static const QUMethod slot_86 = {"setFog", 0, 0 };
    static const QUMethod slot_87 = {"InvertKeyData", 0, 0 };
    static const QUMethod slot_88 = {"updateSliders", 0, 0 };
    static const QUMethod slot_89 = {"slotUpdateListViewCheckBoxes", 0, 0 };
    static const QUMethod slot_90 = {"updateSliderValues", 0, 0 };
    static const QUParameter param_slot_91[] = {
	{ 0, &static_QUType_ptr, "motionNode", QUParameter::In }
    };
    static const QUMethod slot_91 = {"updateSliders", 1, param_slot_91 };
    static const QUMethod slot_92 = {"updateUiOptions", 0, 0 };
    static const QUMethod slot_93 = {"updateUiSettings", 0, 0 };
    static const QUMethod slot_94 = {"hideAllHeadings", 0, 0 };
    static const QUMethod slot_95 = {"showAllHeadings", 0, 0 };
    static const QUParameter param_slot_96[] = {
	{ "ns", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_96 = {"showHeadings", 1, param_slot_96 };
    static const QUParameter param_slot_97[] = {
	{ "heading", &static_QUType_int, 0, QUParameter::In },
	{ "value", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_97 = {"setHeadingValue", 2, param_slot_97 };
    static const QUMethod slot_98 = {"setDefaultHeadings", 0, 0 };
    static const QUMethod slot_99 = {"hideAllSliders", 0, 0 };
    static const QUParameter param_slot_100[] = {
	{ "ns", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_100 = {"hideSliders", 1, param_slot_100 };
    static const QUMethod slot_101 = {"showAllSliders", 0, 0 };
    static const QUParameter param_slot_102[] = {
	{ "ns", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_102 = {"showSliders", 1, param_slot_102 };
    static const QUParameter param_slot_103[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_103 = {"showSlider", 2, param_slot_103 };
    static const QUParameter param_slot_104[] = {
	{ "slider", &static_QUType_int, 0, QUParameter::In },
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_104 = {"setSliderValue", 2, param_slot_104 };
    static const QUParameter param_slot_105[] = {
	{ "slider", &static_QUType_int, 0, QUParameter::In },
	{ "value", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_105 = {"setSliderLabelValue", 2, param_slot_105 };
    static const QUMethod slot_106 = {"setDefaultSliderText", 0, 0 };
    static const QUParameter param_slot_107[] = {
	{ "ns", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_107 = {"showOptions", 1, param_slot_107 };
    static const QUParameter param_slot_108[] = {
	{ "ns", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_108 = {"hideOptions", 1, param_slot_108 };
    static const QUParameter param_slot_109[] = {
	{ "option", &static_QUType_int, 0, QUParameter::In },
	{ "value", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_109 = {"setOptionValue", 2, param_slot_109 };
    static const QUMethod slot_110 = {"sliderselected", 0, 0 };
    static const QUMethod slot_111 = {"slotTimeSliderReleased", 0, 0 };
    static const QUParameter param_slot_112[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_112 = {"updateTimeSlider", 1, param_slot_112 };
    static const QUParameter param_slot_113[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_113 = {"sliderValueChanged", 1, param_slot_113 };
    static const QUParameter param_slot_114[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_114 = {"updatesliderStartframe", 1, param_slot_114 };
    static const QUParameter param_slot_115[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_115 = {"updatesliderEndframe", 1, param_slot_115 };
    static const QUMethod slot_116 = {"toggleForcePlay", 0, 0 };
    static const QUParameter param_slot_117[] = {
	{ "status", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_117 = {"setForcePlay", 1, param_slot_117 };
    static const QUMethod slot_118 = {"firstframeanimation", 0, 0 };
    static const QUMethod slot_119 = {"previousframeanimation", 0, 0 };
    static const QUMethod slot_120 = {"nextframeanimation", 0, 0 };
    static const QUMethod slot_121 = {"lastframeanimation", 0, 0 };
    static const QUMethod slot_122 = {"startanimation", 0, 0 };
    static const QUMethod slot_123 = {"stopanimation", 0, 0 };
    static const QUParameter param_slot_124[] = {
	{ "value", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_124 = {"toggleAnimation", 1, param_slot_124 };
    static const QUParameter param_slot_125[] = {
	{ "Frame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_125 = {"updateAnimation", 1, param_slot_125 };
    static const QUParameter param_slot_126[] = {
	{ "Frame", &static_QUType_int, 0, QUParameter::In },
	{ "force", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_126 = {"updateAnimation", 2, param_slot_126 };
    static const QUMethod slot_127 = {"addKeyframe", 0, 0 };
    static const QUMethod slot_128 = {"removeKeyframe", 0, 0 };
    static const QUMethod slot_129 = {"prevKeyframe", 0, 0 };
    static const QUMethod slot_130 = {"nextKeyframe", 0, 0 };
    static const QUMethod slot_131 = {"slotKeyframeChanged", 0, 0 };
    static const QUMethod slot_132 = {"updateKeyframeDisplay", 0, 0 };
    static const QUParameter param_slot_133[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "jah_layer", &static_QUType_ptr, "JahLayer", QUParameter::In }
    };
    static const QUMethod slot_133 = {"isLayerVisible", 2, param_slot_133 };
    static const QUParameter param_slot_134[] = {
	{ 0, &static_QUType_QVariant, "\x0f", QUParameter::Out },
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_134 = {"Photo", 2, param_slot_134 };
    static const QUParameter param_slot_135[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::Out },
	{ "frameval", &static_QUType_int, 0, QUParameter::In },
	{ "theclip", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_135 = {"buildFrameName", 3, param_slot_135 };
    static const QUParameter param_slot_136[] = {
	{ 0, &static_QUType_QVariant, "\x0f", QUParameter::Out },
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_136 = {"RenderAll", 4, param_slot_136 };
    static const QUMethod slot_137 = {"renderGL", 0, 0 };
    static const QUMethod slot_138 = {"Render", 0, 0 };
    static const QUMethod slot_139 = {"RenderScene", 0, 0 };
    static const QUParameter param_slot_140[] = {
	{ "renStatus", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_140 = {"setSceneToRender", 1, param_slot_140 };
    static const QUParameter param_slot_141[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::Out },
	{ "destDir", &static_QUType_QString, 0, QUParameter::In },
	{ "clipnum", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_141 = {"checkandcleandir", 3, param_slot_141 };
    static const QUMethod slot_142 = {"checkRenderQuality", 0, 0 };
    static const QUMethod slot_143 = {"checkJahRenderer", 0, 0 };
    static const QUParameter param_slot_144[] = {
	{ "quality", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_144 = {"setRenderQuality", 1, param_slot_144 };
    static const QUMethod slot_145 = {"SceneLoad", 0, 0 };
    static const QUMethod slot_146 = {"SceneAppend", 0, 0 };
    static const QUMethod slot_147 = {"importFx", 0, 0 };
    static const QUMethod slot_148 = {"saveEffects", 0, 0 };
    static const QUParameter param_slot_149[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_149 = {"SceneLoadName", 2, param_slot_149 };
    static const QUParameter param_slot_150[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_150 = {"ModelLoadName", 2, param_slot_150 };
    static const QUMethod slot_151 = {"sceneSave", 0, 0 };
    static const QUMethod slot_152 = {"PackageSave", 0, 0 };
    static const QUMethod slot_153 = {"getScript", 0, 0 };
    static const QUMethod slot_154 = {"stopScript", 0, 0 };
    static const QUMethod slot_155 = {"saveAsExport", 0, 0 };
    static const QUParameter param_slot_156[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_156 = {"SetVideoRender", 1, param_slot_156 };
    static const QUParameter param_slot_157[] = {
	{ 0, &static_QUType_int, 0, QUParameter::InOut },
	{ 0, &static_QUType_int, 0, QUParameter::InOut }
    };
    static const QUMethod slot_157 = {"getRenderResolution", 2, param_slot_157 };
    static const QUParameter param_slot_158[] = {
	{ 0, &static_QUType_varptr, "\x0f", QUParameter::InOut }
    };
    static const QUMethod slot_158 = {"SoftPhoto", 1, param_slot_158 };
    static const QUParameter param_slot_159[] = {
	{ "jah_layer", &static_QUType_ptr, "JahLayer", QUParameter::In },
	{ "clip", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_159 = {"addClip", 2, param_slot_159 };
    static const QUParameter param_slot_160[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_160 = {"addClip", 1, param_slot_160 };
    static const QUParameter param_slot_161[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_161 = {"addClipKey", 1, param_slot_161 };
    static const QUMethod slot_162 = {"nameLayer", 0, 0 };
    static const QUMethod slot_163 = {"nameLayerFromTopMenu", 0, 0 };
    static const QUMethod slot_164 = {"ResetAll", 0, 0 };
    static const QUMethod slot_165 = {"ResetLayer", 0, 0 };
    static const QUMethod slot_166 = {"ClearAll", 0, 0 };
    static const QUMethod slot_167 = {"loadClearAll", 0, 0 };
    static const QUParameter param_slot_168[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_168 = {"setlayerName", 1, param_slot_168 };
    static const QUMethod slot_169 = {"toggleLight", 0, 0 };
    static const QUMethod slot_170 = {"toggleMesh", 0, 0 };
    static const QUMethod slot_171 = {"setUsePbuffer", 0, 0 };
    static const QUMethod slot_172 = {"toggleExtendHeadTail", 0, 0 };
    static const QUMethod slot_173 = {"toggleLoop", 0, 0 };
    static const QUMethod slot_174 = {"togglePingPong", 0, 0 };
    static const QUMethod slot_175 = {"toggleShader1", 0, 0 };
    static const QUMethod slot_176 = {"toggleShader2", 0, 0 };
    static const QUMethod slot_177 = {"toggleReflect", 0, 0 };
    static const QUMethod slot_178 = {"toggleSmooth", 0, 0 };
    static const QUMethod slot_179 = {"slotShowLights", 0, 0 };
    static const QUMethod slot_180 = {"changeParticleColors", 0, 0 };
    static const QUMethod slot_181 = {"toggleStatsDisplay", 0, 0 };
    static const QUParameter param_slot_182[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "listv", &static_QUType_ptr, "QListView", QUParameter::In },
	{ "layer", &static_QUType_int, 0, QUParameter::InOut },
	{ "effect", &static_QUType_int, 0, QUParameter::InOut },
	{ "status", &static_QUType_bool, 0, QUParameter::InOut }
    };
    static const QUMethod slot_182 = {"getSelectionTree", 5, param_slot_182 };
    static const QUMethod slot_183 = {"addSelectionFx", 0, 0 };
    static const QUParameter param_slot_184[] = {
	{ "status", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_184 = {"addSelectionFx", 1, param_slot_184 };
    static const QUParameter param_slot_185[] = {
	{ "status", &static_QUType_bool, 0, QUParameter::In },
	{ "fxname", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_185 = {"addSelectionFx", 2, param_slot_185 };
    static const QUParameter param_slot_186[] = {
	{ "status", &static_QUType_bool, 0, QUParameter::In },
	{ "fxname", &static_QUType_QString, 0, QUParameter::In },
	{ "headtail", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_186 = {"addSelectionFx", 3, param_slot_186 };
    static const QUParameter param_slot_187[] = {
	{ "status", &static_QUType_bool, 0, QUParameter::In },
	{ "fxname", &static_QUType_QString, 0, QUParameter::In },
	{ "headtail", &static_QUType_bool, 0, QUParameter::In },
	{ "set_selected", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_187 = {"addSelectionFx", 4, param_slot_187 };
    static const QUParameter param_slot_188[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::Out },
	{ "parent", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "name", &static_QUType_QString, 0, QUParameter::InOut },
	{ "checked", &static_QUType_bool, 0, QUParameter::In },
	{ "selected", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_188 = {"addEffectToUserInterface", 5, param_slot_188 };
    static const QUParameter param_slot_189[] = {
	{ "thelv", &static_QUType_ptr, "QListView", QUParameter::In }
    };
    static const QUMethod slot_189 = {"clearSelection", 1, param_slot_189 };
    static const QUParameter param_slot_190[] = {
	{ "layer", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_190 = {"changeSelection", 1, param_slot_190 };
    static const QUParameter param_slot_191[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_191 = {"setLayerVisible", 2, param_slot_191 };
    static const QUMethod slot_192 = {"ChangeObjectLayer", 0, 0 };
    static const QUMethod slot_193 = {"ChangeObjectCube", 0, 0 };
    static const QUMethod slot_194 = {"ChangeObjectCylinder", 0, 0 };
    static const QUMethod slot_195 = {"ChangeObjectSphere", 0, 0 };
    static const QUMethod slot_196 = {"ChangeObjectBezier", 0, 0 };
    static const QUMethod slot_197 = {"ChangeObjectMesh", 0, 0 };
    static const QUMethod slot_198 = {"delLayer", 0, 0 };
    static const QUParameter param_slot_199[] = {
	{ "layer", &static_QUType_ptr, "JahLayer", QUParameter::In }
    };
    static const QUMethod slot_199 = {"deleteEffectsList", 1, param_slot_199 };
    static const QUParameter param_slot_200[] = {
	{ "layer", &static_QUType_ptr, "JahLayer", QUParameter::In }
    };
    static const QUMethod slot_200 = {"deleteLightsList", 1, param_slot_200 };
    static const QUMethod slot_201 = {"moveLayerUp", 0, 0 };
    static const QUMethod slot_202 = {"moveLayerDown", 0, 0 };
    static const QUParameter param_slot_203[] = {
	{ "layer", &static_QUType_ptr, "JahLayer", QUParameter::In },
	{ "theframe", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_203 = {"addkeyNodes", 2, param_slot_203 };
    static const QUParameter param_slot_204[] = {
	{ "layer", &static_QUType_ptr, "JahLayer", QUParameter::In },
	{ "theframe", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_204 = {"removekeyNodes", 2, param_slot_204 };
    static const QUParameter param_slot_205[] = {
	{ "layer", &static_QUType_ptr, "JahLayer", QUParameter::In },
	{ "currentFrame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_205 = {"updatekeyNodes", 2, param_slot_205 };
    static const QUMethod slot_206 = {"updateKeyNodes", 0, 0 };
    static const QUParameter param_slot_207[] = {
	{ "layer", &static_QUType_ptr, "JahLayer", QUParameter::In }
    };
    static const QUMethod slot_207 = {"clearkeys", 1, param_slot_207 };
    static const QUParameter param_slot_208[] = {
	{ "layer", &static_QUType_ptr, "JahLayer", QUParameter::In }
    };
    static const QUMethod slot_208 = {"resetkeys", 1, param_slot_208 };
    static const QUMethod slot_209 = {"autoSaveThisModule", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "scheduleUpdate()", &slot_0, QMetaData::Public },
	{ "slotUpdateAnimation()", &slot_1, QMetaData::Public },
	{ "slotSetUseOpenGlKeyer()", &slot_2, QMetaData::Public },
	{ "slotSetUseFastShaderKeyer()", &slot_3, QMetaData::Public },
	{ "slotSetSrcBlendFactor()", &slot_4, QMetaData::Public },
	{ "slotSetDstBlendFactor()", &slot_5, QMetaData::Public },
	{ "slotResetBlendFactor()", &slot_6, QMetaData::Public },
	{ "slotSetCompositingMode()", &slot_7, QMetaData::Public },
	{ "slotLightColorAmbientRed()", &slot_8, QMetaData::Public },
	{ "slotLightColorAmbientGreen()", &slot_9, QMetaData::Public },
	{ "slotLightColorAmbientBlue()", &slot_10, QMetaData::Public },
	{ "slotLightColorDiffuseRed()", &slot_11, QMetaData::Public },
	{ "slotLightColorDiffuseGreen()", &slot_12, QMetaData::Public },
	{ "slotLightColorDiffuseBlue()", &slot_13, QMetaData::Public },
	{ "slotLightColorSpecularRed()", &slot_14, QMetaData::Public },
	{ "slotLightColorSpecularGreen()", &slot_15, QMetaData::Public },
	{ "slotLightColorSpecularBlue()", &slot_16, QMetaData::Public },
	{ "slotLightPosition()", &slot_17, QMetaData::Public },
	{ "slotLightColorAmbient(QColor)", &slot_18, QMetaData::Public },
	{ "slotLightColorDiffuse(QColor)", &slot_19, QMetaData::Public },
	{ "slotLightColorSpecular(QColor)", &slot_20, QMetaData::Public },
	{ "slotChooseFramesPerSecond()", &slot_21, QMetaData::Public },
	{ "slotSetShowLights()", &slot_22, QMetaData::Public },
	{ "grabDesktop()", &slot_23, QMetaData::Public },
	{ "paintGL()", &slot_24, QMetaData::Public },
	{ "layerClicked(QListViewItem*)", &slot_25, QMetaData::Public },
	{ "effectNodeClicked(int)", &slot_26, QMetaData::Public },
	{ "mousePressEvent(QMouseEvent*)", &slot_27, QMetaData::Public },
	{ "mouseReleaseEvent(QMouseEvent*)", &slot_28, QMetaData::Public },
	{ "mouseMoveEvent(QMouseEvent*)", &slot_29, QMetaData::Public },
	{ "setSelectColorsIndividually()", &slot_30, QMetaData::Public },
	{ "connectKeyframer()", &slot_31, QMetaData::Public },
	{ "addKeyframer(QWidget*)", &slot_32, QMetaData::Public },
	{ "setResolution(int)", &slot_33, QMetaData::Public },
	{ "updateResMenu(int)", &slot_34, QMetaData::Public },
	{ "setXRotation(int)", &slot_35, QMetaData::Public },
	{ "setYRotation(int)", &slot_36, QMetaData::Public },
	{ "setZRotation(int)", &slot_37, QMetaData::Public },
	{ "setScaleValue(int)", &slot_38, QMetaData::Public },
	{ "getScaleValue(float)", &slot_39, QMetaData::Public },
	{ "setXScale(int)", &slot_40, QMetaData::Public },
	{ "setYScale(int)", &slot_41, QMetaData::Public },
	{ "setZScale(int)", &slot_42, QMetaData::Public },
	{ "setXTrans(int)", &slot_43, QMetaData::Public },
	{ "setYTrans(int)", &slot_44, QMetaData::Public },
	{ "setZTrans(int)", &slot_45, QMetaData::Public },
	{ "setAlpha(int)", &slot_46, QMetaData::Public },
	{ "setZoom(int)", &slot_47, QMetaData::Public },
	{ "setExtrude(int)", &slot_48, QMetaData::Public },
	{ "setColor()", &slot_49, QMetaData::Public },
	{ "setKeyframeDrawStatus()", &slot_50, QMetaData::Public },
	{ "slotSetLayerSelectedVisible()", &slot_51, QMetaData::Public },
	{ "resetAllSliders()", &slot_52, QMetaData::Public },
	{ "spaceEvent(double,double,double,double,double,double)", &slot_53, QMetaData::Public },
	{ "loadObjObject()", &slot_54, QMetaData::Public },
	{ "slotSetSlipFrames(int)", &slot_55, QMetaData::Public },
	{ "setinFrames(int)", &slot_56, QMetaData::Public },
	{ "setoutFrames(int)", &slot_57, QMetaData::Public },
	{ "slotSetKeySlipFrames(int)", &slot_58, QMetaData::Public },
	{ "setinKeyFrames(int)", &slot_59, QMetaData::Public },
	{ "setoutKeyFrames(int)", &slot_60, QMetaData::Public },
	{ "noTool()", &slot_61, QMetaData::Public },
	{ "toolTranslate()", &slot_62, QMetaData::Public },
	{ "toolScale()", &slot_63, QMetaData::Public },
	{ "toolRotate()", &slot_64, QMetaData::Public },
	{ "changeZoom(int)", &slot_65, QMetaData::Public },
	{ "resetZoom()", &slot_66, QMetaData::Public },
	{ "setZoom(GLfloat)", &slot_67, QMetaData::Public },
	{ "setHFog(int)", &slot_68, QMetaData::Public },
	{ "setVFog(int)", &slot_69, QMetaData::Public },
	{ "updateFog()", &slot_70, QMetaData::Public },
	{ "SetGPUSelect()", &slot_71, QMetaData::Public },
	{ "updateToolDisplay()", &slot_72, QMetaData::Public },
	{ "setKeyStatus()", &slot_73, QMetaData::Public },
	{ "setClipStatus()", &slot_74, QMetaData::Public },
	{ "toggleForegroundStatus()", &slot_75, QMetaData::Public },
	{ "toggleDepthStatus()", &slot_76, QMetaData::Public },
	{ "setTextLayerText(JahLayer*,const char*)", &slot_77, QMetaData::Public },
	{ "setText(const char*)", &slot_78, QMetaData::Public },
	{ "setText(const QString&)", &slot_79, QMetaData::Public },
	{ "setFontColor()", &slot_80, QMetaData::Public },
	{ "loadFont()", &slot_81, QMetaData::Public },
	{ "setTextSafe()", &slot_82, QMetaData::Public },
	{ "setGrid()", &slot_83, QMetaData::Public },
	{ "setSmooth()", &slot_84, QMetaData::Public },
	{ "setBlur()", &slot_85, QMetaData::Public },
	{ "setFog()", &slot_86, QMetaData::Public },
	{ "InvertKeyData()", &slot_87, QMetaData::Public },
	{ "updateSliders()", &slot_88, QMetaData::Public },
	{ "slotUpdateListViewCheckBoxes()", &slot_89, QMetaData::Public },
	{ "updateSliderValues()", &slot_90, QMetaData::Public },
	{ "updateSliders(motionNode*)", &slot_91, QMetaData::Public },
	{ "updateUiOptions()", &slot_92, QMetaData::Public },
	{ "updateUiSettings()", &slot_93, QMetaData::Public },
	{ "hideAllHeadings()", &slot_94, QMetaData::Public },
	{ "showAllHeadings()", &slot_95, QMetaData::Public },
	{ "showHeadings(int)", &slot_96, QMetaData::Public },
	{ "setHeadingValue(int,QString)", &slot_97, QMetaData::Public },
	{ "setDefaultHeadings()", &slot_98, QMetaData::Public },
	{ "hideAllSliders()", &slot_99, QMetaData::Public },
	{ "hideSliders(int)", &slot_100, QMetaData::Public },
	{ "showAllSliders()", &slot_101, QMetaData::Public },
	{ "showSliders(int)", &slot_102, QMetaData::Public },
	{ "showSlider(int,bool)", &slot_103, QMetaData::Public },
	{ "setSliderValue(int,int)", &slot_104, QMetaData::Public },
	{ "setSliderLabelValue(int,QString)", &slot_105, QMetaData::Public },
	{ "setDefaultSliderText()", &slot_106, QMetaData::Public },
	{ "showOptions(int)", &slot_107, QMetaData::Public },
	{ "hideOptions(int)", &slot_108, QMetaData::Public },
	{ "setOptionValue(int,bool)", &slot_109, QMetaData::Public },
	{ "sliderselected()", &slot_110, QMetaData::Public },
	{ "slotTimeSliderReleased()", &slot_111, QMetaData::Public },
	{ "updateTimeSlider(int)", &slot_112, QMetaData::Public },
	{ "sliderValueChanged(int)", &slot_113, QMetaData::Public },
	{ "updatesliderStartframe(int)", &slot_114, QMetaData::Public },
	{ "updatesliderEndframe(int)", &slot_115, QMetaData::Public },
	{ "toggleForcePlay()", &slot_116, QMetaData::Public },
	{ "setForcePlay(bool)", &slot_117, QMetaData::Public },
	{ "firstframeanimation()", &slot_118, QMetaData::Public },
	{ "previousframeanimation()", &slot_119, QMetaData::Public },
	{ "nextframeanimation()", &slot_120, QMetaData::Public },
	{ "lastframeanimation()", &slot_121, QMetaData::Public },
	{ "startanimation()", &slot_122, QMetaData::Public },
	{ "stopanimation()", &slot_123, QMetaData::Public },
	{ "toggleAnimation(bool)", &slot_124, QMetaData::Public },
	{ "updateAnimation(int)", &slot_125, QMetaData::Public },
	{ "updateAnimation(int,bool)", &slot_126, QMetaData::Public },
	{ "addKeyframe()", &slot_127, QMetaData::Public },
	{ "removeKeyframe()", &slot_128, QMetaData::Public },
	{ "prevKeyframe()", &slot_129, QMetaData::Public },
	{ "nextKeyframe()", &slot_130, QMetaData::Public },
	{ "slotKeyframeChanged()", &slot_131, QMetaData::Public },
	{ "updateKeyframeDisplay()", &slot_132, QMetaData::Public },
	{ "isLayerVisible(JahLayer*)", &slot_133, QMetaData::Public },
	{ "Photo(assetData)", &slot_134, QMetaData::Public },
	{ "buildFrameName(int,assetData)", &slot_135, QMetaData::Public },
	{ "RenderAll(assetData,int,int)", &slot_136, QMetaData::Public },
	{ "renderGL()", &slot_137, QMetaData::Public },
	{ "Render()", &slot_138, QMetaData::Public },
	{ "RenderScene()", &slot_139, QMetaData::Public },
	{ "setSceneToRender(bool)", &slot_140, QMetaData::Public },
	{ "checkandcleandir(QString,int)", &slot_141, QMetaData::Public },
	{ "checkRenderQuality()", &slot_142, QMetaData::Public },
	{ "checkJahRenderer()", &slot_143, QMetaData::Public },
	{ "setRenderQuality(int)", &slot_144, QMetaData::Public },
	{ "SceneLoad()", &slot_145, QMetaData::Public },
	{ "SceneAppend()", &slot_146, QMetaData::Public },
	{ "importFx()", &slot_147, QMetaData::Public },
	{ "saveEffects()", &slot_148, QMetaData::Public },
	{ "SceneLoadName(QString)", &slot_149, QMetaData::Public },
	{ "ModelLoadName(QString)", &slot_150, QMetaData::Public },
	{ "sceneSave()", &slot_151, QMetaData::Public },
	{ "PackageSave()", &slot_152, QMetaData::Public },
	{ "getScript()", &slot_153, QMetaData::Public },
	{ "stopScript()", &slot_154, QMetaData::Public },
	{ "saveAsExport()", &slot_155, QMetaData::Public },
	{ "SetVideoRender(bool)", &slot_156, QMetaData::Public },
	{ "getRenderResolution(int&,int&)", &slot_157, QMetaData::Public },
	{ "SoftPhoto(QImage&)", &slot_158, QMetaData::Public },
	{ "addClip(JahLayer*,assetData)", &slot_159, QMetaData::Public },
	{ "addClip(assetData)", &slot_160, QMetaData::Public },
	{ "addClipKey(assetData)", &slot_161, QMetaData::Public },
	{ "nameLayer()", &slot_162, QMetaData::Public },
	{ "nameLayerFromTopMenu()", &slot_163, QMetaData::Public },
	{ "ResetAll()", &slot_164, QMetaData::Public },
	{ "ResetLayer()", &slot_165, QMetaData::Public },
	{ "ClearAll()", &slot_166, QMetaData::Public },
	{ "loadClearAll()", &slot_167, QMetaData::Public },
	{ "setlayerName(QString)", &slot_168, QMetaData::Public },
	{ "toggleLight()", &slot_169, QMetaData::Public },
	{ "toggleMesh()", &slot_170, QMetaData::Public },
	{ "setUsePbuffer()", &slot_171, QMetaData::Public },
	{ "toggleExtendHeadTail()", &slot_172, QMetaData::Public },
	{ "toggleLoop()", &slot_173, QMetaData::Public },
	{ "togglePingPong()", &slot_174, QMetaData::Public },
	{ "toggleShader1()", &slot_175, QMetaData::Public },
	{ "toggleShader2()", &slot_176, QMetaData::Public },
	{ "toggleReflect()", &slot_177, QMetaData::Public },
	{ "toggleSmooth()", &slot_178, QMetaData::Public },
	{ "slotShowLights()", &slot_179, QMetaData::Public },
	{ "changeParticleColors()", &slot_180, QMetaData::Public },
	{ "toggleStatsDisplay()", &slot_181, QMetaData::Public },
	{ "getSelectionTree(QListView*,int&,int&,bool&)", &slot_182, QMetaData::Public },
	{ "addSelectionFx()", &slot_183, QMetaData::Public },
	{ "addSelectionFx(bool)", &slot_184, QMetaData::Public },
	{ "addSelectionFx(bool,QString)", &slot_185, QMetaData::Public },
	{ "addSelectionFx(bool,QString,bool)", &slot_186, QMetaData::Public },
	{ "addSelectionFx(bool,QString,bool,bool)", &slot_187, QMetaData::Public },
	{ "addEffectToUserInterface(QListViewItem*,QString&,bool,bool)", &slot_188, QMetaData::Public },
	{ "clearSelection(QListView*)", &slot_189, QMetaData::Public },
	{ "changeSelection(int)", &slot_190, QMetaData::Public },
	{ "setLayerVisible(int,bool)", &slot_191, QMetaData::Public },
	{ "ChangeObjectLayer()", &slot_192, QMetaData::Public },
	{ "ChangeObjectCube()", &slot_193, QMetaData::Public },
	{ "ChangeObjectCylinder()", &slot_194, QMetaData::Public },
	{ "ChangeObjectSphere()", &slot_195, QMetaData::Public },
	{ "ChangeObjectBezier()", &slot_196, QMetaData::Public },
	{ "ChangeObjectMesh()", &slot_197, QMetaData::Public },
	{ "delLayer()", &slot_198, QMetaData::Public },
	{ "deleteEffectsList(JahLayer*)", &slot_199, QMetaData::Public },
	{ "deleteLightsList(JahLayer*)", &slot_200, QMetaData::Public },
	{ "moveLayerUp()", &slot_201, QMetaData::Public },
	{ "moveLayerDown()", &slot_202, QMetaData::Public },
	{ "addkeyNodes(JahLayer*,int)", &slot_203, QMetaData::Public },
	{ "removekeyNodes(JahLayer*,int)", &slot_204, QMetaData::Public },
	{ "updatekeyNodes(JahLayer*,int)", &slot_205, QMetaData::Public },
	{ "updateKeyNodes()", &slot_206, QMetaData::Public },
	{ "clearkeys(JahLayer*)", &slot_207, QMetaData::Public },
	{ "resetkeys(JahLayer*)", &slot_208, QMetaData::Public },
	{ "autoSaveThisModule()", &slot_209, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_0 = {"updateDesktop", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "debugmessage", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"updateBrowser", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"hideUI", 1, param_signal_2 };
    static const QUMethod signal_3 = {"GrabDesktop", 0, 0 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_ptr, "JahHeadable", QUParameter::In }
    };
    static const QUMethod signal_4 = {"showInHead", 1, param_signal_4 };
    static const QMetaData signal_tbl[] = {
	{ "updateDesktop(assetData)", &signal_0, QMetaData::Public },
	{ "updateBrowser(QString)", &signal_1, QMetaData::Public },
	{ "hideUI(bool)", &signal_2, QMetaData::Public },
	{ "GrabDesktop()", &signal_3, QMetaData::Public },
	{ "showInHead(JahHeadable*)", &signal_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLWorld", parentObject,
	slot_tbl, 210,
	signal_tbl, 5,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLWorld.setMetaObject( metaObj );
    return metaObj;
}

void* GLWorld::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLWorld" ) )
	return this;
    if ( !qstrcmp( clname, "JahHeadable" ) )
	return (JahHeadable*)this;
    return QWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL updateDesktop
void GLWorld::updateDesktop( assetData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL updateBrowser
void GLWorld::updateBrowser( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL hideUI
void GLWorld::hideUI( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL GrabDesktop
void GLWorld::GrabDesktop()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

// SIGNAL showInHead
void GLWorld::showInHead( JahHeadable* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 4 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

bool GLWorld::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: scheduleUpdate(); break;
    case 1: slotUpdateAnimation(); break;
    case 2: slotSetUseOpenGlKeyer(); break;
    case 3: slotSetUseFastShaderKeyer(); break;
    case 4: slotSetSrcBlendFactor(); break;
    case 5: slotSetDstBlendFactor(); break;
    case 6: slotResetBlendFactor(); break;
    case 7: slotSetCompositingMode(); break;
    case 8: slotLightColorAmbientRed(); break;
    case 9: slotLightColorAmbientGreen(); break;
    case 10: slotLightColorAmbientBlue(); break;
    case 11: slotLightColorDiffuseRed(); break;
    case 12: slotLightColorDiffuseGreen(); break;
    case 13: slotLightColorDiffuseBlue(); break;
    case 14: slotLightColorSpecularRed(); break;
    case 15: slotLightColorSpecularGreen(); break;
    case 16: slotLightColorSpecularBlue(); break;
    case 17: slotLightPosition(); break;
    case 18: slotLightColorAmbient((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    case 19: slotLightColorDiffuse((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    case 20: slotLightColorSpecular((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    case 21: slotChooseFramesPerSecond(); break;
    case 22: slotSetShowLights(); break;
    case 23: grabDesktop(); break;
    case 24: paintGL(); break;
    case 25: layerClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 26: effectNodeClicked((int)static_QUType_int.get(_o+1)); break;
    case 27: mousePressEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 28: mouseReleaseEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 29: mouseMoveEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 30: setSelectColorsIndividually(); break;
    case 31: connectKeyframer(); break;
    case 32: addKeyframer((QWidget*)static_QUType_ptr.get(_o+1)); break;
    case 33: setResolution((int)static_QUType_int.get(_o+1)); break;
    case 34: updateResMenu((int)static_QUType_int.get(_o+1)); break;
    case 35: setXRotation((int)static_QUType_int.get(_o+1)); break;
    case 36: setYRotation((int)static_QUType_int.get(_o+1)); break;
    case 37: setZRotation((int)static_QUType_int.get(_o+1)); break;
    case 38: setScaleValue((int)static_QUType_int.get(_o+1)); break;
    case 39: static_QUType_int.set(_o,getScaleValue((float)(*((float*)static_QUType_ptr.get(_o+1))))); break;
    case 40: setXScale((int)static_QUType_int.get(_o+1)); break;
    case 41: setYScale((int)static_QUType_int.get(_o+1)); break;
    case 42: setZScale((int)static_QUType_int.get(_o+1)); break;
    case 43: setXTrans((int)static_QUType_int.get(_o+1)); break;
    case 44: setYTrans((int)static_QUType_int.get(_o+1)); break;
    case 45: setZTrans((int)static_QUType_int.get(_o+1)); break;
    case 46: setAlpha((int)static_QUType_int.get(_o+1)); break;
    case 47: setZoom((int)static_QUType_int.get(_o+1)); break;
    case 48: setExtrude((int)static_QUType_int.get(_o+1)); break;
    case 49: setColor(); break;
    case 50: setKeyframeDrawStatus(); break;
    case 51: slotSetLayerSelectedVisible(); break;
    case 52: resetAllSliders(); break;
    case 53: spaceEvent((double)static_QUType_double.get(_o+1),(double)static_QUType_double.get(_o+2),(double)static_QUType_double.get(_o+3),(double)static_QUType_double.get(_o+4),(double)static_QUType_double.get(_o+5),(double)static_QUType_double.get(_o+6)); break;
    case 54: loadObjObject(); break;
    case 55: slotSetSlipFrames((int)static_QUType_int.get(_o+1)); break;
    case 56: setinFrames((int)static_QUType_int.get(_o+1)); break;
    case 57: setoutFrames((int)static_QUType_int.get(_o+1)); break;
    case 58: slotSetKeySlipFrames((int)static_QUType_int.get(_o+1)); break;
    case 59: setinKeyFrames((int)static_QUType_int.get(_o+1)); break;
    case 60: setoutKeyFrames((int)static_QUType_int.get(_o+1)); break;
    case 61: noTool(); break;
    case 62: toolTranslate(); break;
    case 63: toolScale(); break;
    case 64: toolRotate(); break;
    case 65: changeZoom((int)static_QUType_int.get(_o+1)); break;
    case 66: resetZoom(); break;
    case 67: setZoom((GLfloat)(*((GLfloat*)static_QUType_ptr.get(_o+1)))); break;
    case 68: setHFog((int)static_QUType_int.get(_o+1)); break;
    case 69: setVFog((int)static_QUType_int.get(_o+1)); break;
    case 70: updateFog(); break;
    case 71: SetGPUSelect(); break;
    case 72: updateToolDisplay(); break;
    case 73: setKeyStatus(); break;
    case 74: setClipStatus(); break;
    case 75: toggleForegroundStatus(); break;
    case 76: toggleDepthStatus(); break;
    case 77: setTextLayerText((JahLayer*)static_QUType_ptr.get(_o+1),(const char*)static_QUType_charstar.get(_o+2)); break;
    case 78: setText((const char*)static_QUType_charstar.get(_o+1)); break;
    case 79: setText((const QString&)static_QUType_QString.get(_o+1)); break;
    case 80: setFontColor(); break;
    case 81: loadFont(); break;
    case 82: setTextSafe(); break;
    case 83: setGrid(); break;
    case 84: setSmooth(); break;
    case 85: setBlur(); break;
    case 86: setFog(); break;
    case 87: InvertKeyData(); break;
    case 88: updateSliders(); break;
    case 89: slotUpdateListViewCheckBoxes(); break;
    case 90: updateSliderValues(); break;
    case 91: updateSliders((motionNode*)static_QUType_ptr.get(_o+1)); break;
    case 92: updateUiOptions(); break;
    case 93: updateUiSettings(); break;
    case 94: hideAllHeadings(); break;
    case 95: showAllHeadings(); break;
    case 96: showHeadings((int)static_QUType_int.get(_o+1)); break;
    case 97: setHeadingValue((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 98: setDefaultHeadings(); break;
    case 99: hideAllSliders(); break;
    case 100: hideSliders((int)static_QUType_int.get(_o+1)); break;
    case 101: showAllSliders(); break;
    case 102: showSliders((int)static_QUType_int.get(_o+1)); break;
    case 103: showSlider((int)static_QUType_int.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 104: setSliderValue((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 105: setSliderLabelValue((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 106: setDefaultSliderText(); break;
    case 107: showOptions((int)static_QUType_int.get(_o+1)); break;
    case 108: hideOptions((int)static_QUType_int.get(_o+1)); break;
    case 109: setOptionValue((int)static_QUType_int.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 110: sliderselected(); break;
    case 111: slotTimeSliderReleased(); break;
    case 112: updateTimeSlider((int)static_QUType_int.get(_o+1)); break;
    case 113: sliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 114: updatesliderStartframe((int)static_QUType_int.get(_o+1)); break;
    case 115: updatesliderEndframe((int)static_QUType_int.get(_o+1)); break;
    case 116: toggleForcePlay(); break;
    case 117: setForcePlay((bool)static_QUType_bool.get(_o+1)); break;
    case 118: firstframeanimation(); break;
    case 119: previousframeanimation(); break;
    case 120: nextframeanimation(); break;
    case 121: lastframeanimation(); break;
    case 122: startanimation(); break;
    case 123: stopanimation(); break;
    case 124: toggleAnimation((bool)static_QUType_bool.get(_o+1)); break;
    case 125: updateAnimation((int)static_QUType_int.get(_o+1)); break;
    case 126: updateAnimation((int)static_QUType_int.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 127: addKeyframe(); break;
    case 128: removeKeyframe(); break;
    case 129: prevKeyframe(); break;
    case 130: nextKeyframe(); break;
    case 131: slotKeyframeChanged(); break;
    case 132: updateKeyframeDisplay(); break;
    case 133: static_QUType_bool.set(_o,isLayerVisible((JahLayer*)static_QUType_ptr.get(_o+1))); break;
    case 134: static_QUType_QVariant.set(_o,QVariant(Photo((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))))); break;
    case 135: static_QUType_QString.set(_o,buildFrameName((int)static_QUType_int.get(_o+1),(assetData)(*((assetData*)static_QUType_ptr.get(_o+2))))); break;
    case 136: static_QUType_QVariant.set(_o,QVariant(RenderAll((assetData)(*((assetData*)static_QUType_ptr.get(_o+1))),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)))); break;
    case 137: renderGL(); break;
    case 138: Render(); break;
    case 139: RenderScene(); break;
    case 140: setSceneToRender((bool)static_QUType_bool.get(_o+1)); break;
    case 141: static_QUType_QString.set(_o,checkandcleandir((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2))); break;
    case 142: checkRenderQuality(); break;
    case 143: checkJahRenderer(); break;
    case 144: setRenderQuality((int)static_QUType_int.get(_o+1)); break;
    case 145: SceneLoad(); break;
    case 146: SceneAppend(); break;
    case 147: importFx(); break;
    case 148: saveEffects(); break;
    case 149: static_QUType_bool.set(_o,SceneLoadName((QString)static_QUType_QString.get(_o+1))); break;
    case 150: static_QUType_bool.set(_o,ModelLoadName((QString)static_QUType_QString.get(_o+1))); break;
    case 151: sceneSave(); break;
    case 152: PackageSave(); break;
    case 153: getScript(); break;
    case 154: stopScript(); break;
    case 155: saveAsExport(); break;
    case 156: SetVideoRender((bool)static_QUType_bool.get(_o+1)); break;
    case 157: getRenderResolution((int&)static_QUType_int.get(_o+1),(int&)static_QUType_int.get(_o+2)); break;
    case 158: SoftPhoto((QImage&)*((QImage*)static_QUType_ptr.get(_o+1))); break;
    case 159: addClip((JahLayer*)static_QUType_ptr.get(_o+1),(assetData)(*((assetData*)static_QUType_ptr.get(_o+2)))); break;
    case 160: addClip((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 161: addClipKey((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 162: nameLayer(); break;
    case 163: nameLayerFromTopMenu(); break;
    case 164: ResetAll(); break;
    case 165: ResetLayer(); break;
    case 166: ClearAll(); break;
    case 167: loadClearAll(); break;
    case 168: setlayerName((QString)static_QUType_QString.get(_o+1)); break;
    case 169: toggleLight(); break;
    case 170: toggleMesh(); break;
    case 171: setUsePbuffer(); break;
    case 172: toggleExtendHeadTail(); break;
    case 173: toggleLoop(); break;
    case 174: togglePingPong(); break;
    case 175: toggleShader1(); break;
    case 176: toggleShader2(); break;
    case 177: toggleReflect(); break;
    case 178: toggleSmooth(); break;
    case 179: slotShowLights(); break;
    case 180: changeParticleColors(); break;
    case 181: toggleStatsDisplay(); break;
    case 182: static_QUType_bool.set(_o,getSelectionTree((QListView*)static_QUType_ptr.get(_o+1),(int&)static_QUType_int.get(_o+2),(int&)static_QUType_int.get(_o+3),(bool&)static_QUType_bool.get(_o+4))); break;
    case 183: addSelectionFx(); break;
    case 184: addSelectionFx((bool)static_QUType_bool.get(_o+1)); break;
    case 185: addSelectionFx((bool)static_QUType_bool.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 186: addSelectionFx((bool)static_QUType_bool.get(_o+1),(QString)static_QUType_QString.get(_o+2),(bool)static_QUType_bool.get(_o+3)); break;
    case 187: addSelectionFx((bool)static_QUType_bool.get(_o+1),(QString)static_QUType_QString.get(_o+2),(bool)static_QUType_bool.get(_o+3),(bool)static_QUType_bool.get(_o+4)); break;
    case 188: static_QUType_ptr.set(_o,addEffectToUserInterface((QListViewItem*)static_QUType_ptr.get(_o+1),(QString&)static_QUType_QString.get(_o+2),(bool)static_QUType_bool.get(_o+3),(bool)static_QUType_bool.get(_o+4))); break;
    case 189: clearSelection((QListView*)static_QUType_ptr.get(_o+1)); break;
    case 190: changeSelection((int)static_QUType_int.get(_o+1)); break;
    case 191: setLayerVisible((int)static_QUType_int.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 192: ChangeObjectLayer(); break;
    case 193: ChangeObjectCube(); break;
    case 194: ChangeObjectCylinder(); break;
    case 195: ChangeObjectSphere(); break;
    case 196: ChangeObjectBezier(); break;
    case 197: ChangeObjectMesh(); break;
    case 198: delLayer(); break;
    case 199: deleteEffectsList((JahLayer*)static_QUType_ptr.get(_o+1)); break;
    case 200: deleteLightsList((JahLayer*)static_QUType_ptr.get(_o+1)); break;
    case 201: moveLayerUp(); break;
    case 202: moveLayerDown(); break;
    case 203: addkeyNodes((JahLayer*)static_QUType_ptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 204: removekeyNodes((JahLayer*)static_QUType_ptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 205: updatekeyNodes((JahLayer*)static_QUType_ptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 206: updateKeyNodes(); break;
    case 207: clearkeys((JahLayer*)static_QUType_ptr.get(_o+1)); break;
    case 208: resetkeys((JahLayer*)static_QUType_ptr.get(_o+1)); break;
    case 209: autoSaveThisModule(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLWorld::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: updateDesktop((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 1: updateBrowser((QString)static_QUType_QString.get(_o+1)); break;
    case 2: hideUI((bool)static_QUType_bool.get(_o+1)); break;
    case 3: GrabDesktop(); break;
    case 4: showInHead((JahHeadable*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GLWorld::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool GLWorld::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
