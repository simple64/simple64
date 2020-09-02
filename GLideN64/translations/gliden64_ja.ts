<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja_JP">
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="AboutDialog.ui" line="17"/>
        <source>About GLideN64</source>
        <translation>GLideN64について</translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="80"/>
        <source>About</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="102"/>
        <source>The next generation open source graphics plugin for N64 emulators.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="125"/>
        <source>Authors</source>
        <translation>作者</translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="145"/>
        <source>Developer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="169"/>
        <source>Beta tester</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="193"/>
        <source>GUI designer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="207"/>
        <source>Contributors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="470"/>
        <source>Author of the original glN64 graphics plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="494"/>
        <source>Author of gles2n64, a port of glN64 to GL ES 2.0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="518"/>
        <source>Author of the GlideHQ texture library</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="542"/>
        <source>Author of z64, an OpenGL LLE plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="240"/>
        <source>Funders</source>
        <translation>寄付</translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="438"/>
        <source>... and more</source>
        <extracomment>Refers to more funders</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="AboutDialog.ui" line="450"/>
        <source>Credits</source>
        <translation>クレジット</translation>
    </message>
    <message>
        <location filename="AboutDialog.cpp" line="20"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConfigDialog</name>
    <message>
        <location filename="configDialog.ui" line="37"/>
        <location filename="configDialog.ui" line="48"/>
        <source>Video</source>
        <translation>ビデオ</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="78"/>
        <source>Full screen resolution:</source>
        <translation>フルスクリーン解像度:</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="129"/>
        <source>Windowed resolution:</source>
        <translation>ウィンドウ解像度:</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="90"/>
        <source>Refresh rate:</source>
        <translation>リフレッシュレート:</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="190"/>
        <source>Stretch</source>
        <translation>引き伸ばし</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="185"/>
        <source>16:9</source>
        <translation>16:9</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="108"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option selects the resolution for windowed mode. You can also type in a custom window size.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;640 x 480, 800 x 600, 1024 x 768, 1280 x 960&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="216"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Threaded video can improve performance with poor OpenGL drivers at the cost of very marginal input lag, usually less than half a frame.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Usually off, unless there are performance issues&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="219"/>
        <source>Enable threaded video</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="232"/>
        <source>When enabled, the image is cropped by values specified in N64 pixels. Useful to remove black borders in some games.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="253"/>
        <location filename="ConfigDialog.cpp" line="767"/>
        <source>Overscan</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="267"/>
        <source>NTSC</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="320"/>
        <source>PAL</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="388"/>
        <source>Anti-aliasing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="397"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;GLideN64 offers two methods to smooth jagged polygons:&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Fast approximate anti-aliasing (FXAA)&lt;/span&gt;: FXAA is a post-processing filter that can provide a decent result, but as good as MSAA. The main reason to use FXAA is to use with &lt;span style=&quot; font-weight:600;&quot;&gt;N64-style depth compare&lt;/span&gt;. FXAA adds some blurriness to the output image, causing some textures like text to possibly look worse.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Multisample anti-aliasing (MSAA)&lt;/span&gt;: MSAA is a standard anti-aliasing technique used in computer graphics to improve image quality. Most modern GPUs support 2, 4, 8, and 16 samples. More samples mean better quality, but are slower. There are two downsides: it&apos;s incompatible with &lt;span style=&quot; font-weight:600;&quot;&gt;N64-style depth compare&lt;/span&gt; and may cause minor glitches in some games.&lt;/p&gt;&lt;p&gt;Recommendation: [&lt;span style=&quot; font-style:italic;&quot;&gt;Usually 16x MSAA, or FXAA with N64-style depth compare&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="418"/>
        <source>No anti-aliasing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="425"/>
        <source>Fast approximate anti-aliasing (FXAA)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="432"/>
        <source>Multisample anti-aliasing (MSAA):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="616"/>
        <source>Multisample anti-aliasing is not compatible with &lt;a href=&quot;#n64DepthCompare&quot;&gt;N64-style depth compare&lt;/a&gt;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="638"/>
        <source>Filtering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="662"/>
        <source>Anisotropic filtering:</source>
        <translation>異方性フィルタリング:</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="927"/>
        <source>PNG</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="977"/>
        <source>Language:</source>
        <translation>言語:</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1043"/>
        <location filename="configDialog.ui" line="1743"/>
        <source>Some of the options on this tab have been disabled because &lt;a href=&quot;#frameBuffer&quot;&gt;frame buffer emulation&lt;/a&gt; has been turned off.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1066"/>
        <source>Emulation</source>
        <translation>エミュレーション</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1086"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When enabled, all non-default values of settings are stored individually for each game.&lt;/p&gt;&lt;p&gt;When a game is running, settings are displayed and saved for the currently running game.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Note:&lt;/span&gt; GLideN64 already contains settings for the optimal performance of most games. Be careful when altering options on &apos;Emulation&apos; and &apos;Frame buffer&apos; tab.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Checked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1089"/>
        <source>Use per-game settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1170"/>
        <source>Internal resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1218"/>
        <source>x</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1252"/>
        <source>Gamma</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1263"/>
        <source>Custom gamma correction level:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1355"/>
        <source>Selecting this option overrides gamma correction specified by the game.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1386"/>
        <source>Dithering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1392"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Like real hardware this setting reduces the number of colors if dithering is used. Removes undesired dithering fragments. Default = enabled.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1395"/>
        <source>Enable 5bit quantization</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1402"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This setting doubles noise resolution to get a finer noise effect at higher resolutions. Default = disabled.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1405"/>
        <source>High resolution noise dithering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1412"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This setting enables game controlled ordered grid dithering. Enable it for accurate representation. Default = disabled.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1415"/>
        <source>Dithering pattern on output image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1424"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;RDRAM dithering prevents color banding in games with framebuffer effects.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1427"/>
        <source>RDRAM image dithering:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1434"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Settings: Disabled, Bayer ordered grid dithering, Magic Square ordered grid dithering or blue noise dithering. Blue noise dithering produces unobtrusive results. Default = blue noise dithering.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1438"/>
        <source>disable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1443"/>
        <source>Bayer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1448"/>
        <source>Magic square</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1453"/>
        <source>Blue noise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1466"/>
        <source>2D elements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1497"/>
        <source>Render 2D elements in N64 resolution (best quality, can be slow):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1513"/>
        <source>Enable optimized</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1518"/>
        <source>Enable unoptimized</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1528"/>
        <source>Try to remove colored halos around transparent objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1628"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Background is a complex macro command used to render large (normally full screen) images. Since background images usually don&apos;t fit texture memory, the microcode splits them on narrow strips and renders them one by one. HLE code has two modes to emulate background commands:&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;One piece&lt;/span&gt;: The whole background image rendred as one textured rectangle. This method is normally much faster, but the result is not always correct.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Stripped&lt;/span&gt;: This method emulates background commands as close as possible to actual microcode implementation. It&apos;s slower but more precise. Another problem: some games may have gaps between rendered strips in high resolution. Use &amp;quot;Render 2D elements in N64 resolution&amp;quot; option to remove the gaps.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Game dependent, mostly Stripped&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1649"/>
        <source>Background rendering mode (HLE only):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1656"/>
        <source>One piece</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1663"/>
        <source>Stripped</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1789"/>
        <source>Emulate frame buffer</source>
        <translation>フレームバッファのエミュレート</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2133"/>
        <location filename="configDialog.ui" line="2143"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The N64 uses a unique method of calculating depth to the camera. When enabled, GlideN64 uses shaders to try to emulate these calculations correctly. Not compatible with anti-aliasing. &lt;span style=&quot; font-weight:600;&quot;&gt;Experimental!&lt;/span&gt;&lt;/p&gt;&lt;p&gt;Fast mode requires OpenGL 4.2 and &amp;quot;fragment shader interlock&amp;quot; extensions.&lt;/p&gt;&lt;p&gt;Compatible mode requires only core OpenGL 4.2 &lt;span style=&quot; font-weight:600;&quot;&gt;Can be slow!&lt;/span&gt;&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Sometimes checked, for several games&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2136"/>
        <source>N64-style depth compare (experimental, disables MSAA)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2152"/>
        <source>Fast</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2157"/>
        <source>Compatible</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2683"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option enables alternative storage for hi-res textures.&lt;/p&gt;&lt;p&gt;Normally memory cache is used. It keeps all hi-res textures in RAM and thus limited by available RAM size.&lt;/p&gt;&lt;p&gt;File storage keeps texture cache on HDD. It is slower than memory cache, but has virtually no limits on cache size.&lt;/p&gt;&lt;p&gt;Disable &amp;quot;Compress texture cache&amp;quot; option for better performance.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2696"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option dumps textures on screen to a texture pack folder. &lt;/p&gt;&lt;p&gt;Hotkey:&lt;br/&gt;Use &lt;span style=&quot; font-weight:600;&quot;&gt;D&lt;/span&gt; to toggle texture dumping on or off&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2699"/>
        <source>Press &apos;d&apos; to dump N64 textures (for texture artists)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2706"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option allows texture artists to reload hi-res textures while the game is running to instantly see how they look —big time saver!&lt;/p&gt;&lt;p&gt;Hotkey:&lt;br/&gt;Use &lt;span style=&quot; font-weight:600;&quot;&gt;R&lt;/span&gt; to reload textures from the texture pack&lt;br/&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2709"/>
        <source>Press &apos;r&apos; to reload hi-res textures (for texture artists)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3350"/>
        <source>Display FPS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3357"/>
        <source>Display VI/s</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3371"/>
        <source>Display internal resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3378"/>
        <source>Display rendering resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3402"/>
        <source>Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3458"/>
        <source>Size:</source>
        <extracomment>Font size</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3471"/>
        <source> px</source>
        <extracomment>Short for font size pixels; include a leading space if appropriate</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3534"/>
        <source>████</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3616"/>
        <source>The font preview only shows the regular style for the font family. Some styles like bold and italic display only in game.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3661"/>
        <location filename="ConfigDialog.cpp" line="342"/>
        <source>Debug</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3670"/>
        <source>Dump low-level info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3677"/>
        <source>Dump display list commands</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3684"/>
        <source>Dump detailed info (matrices, vertices, etc.)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3741"/>
        <source>This game</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3748"/>
        <source>Save settings for:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3755"/>
        <location filename="configDialog.ui" line="3765"/>
        <source>Settings profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3785"/>
        <location filename="ConfigDialog.cpp" line="972"/>
        <source>Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1508"/>
        <location filename="configDialog.ui" line="2147"/>
        <source>Disable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="140"/>
        <location filename="ConfigDialog.cpp" line="761"/>
        <source>Custom</source>
        <extracomment>In the dropdown for windowed resolutions</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1830"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Unchecking this option disables many effects including cropping, aspect ratio, N64 resolution factor, N64-style depth compare and more. Don&apos;t uncheck this option unless you have performance issues.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1935"/>
        <source>Vertical interrupt (recommended, fewest game issues)</source>
        <extracomment>&quot;Vertical interrupt&quot; is frequently abbreviated as &quot;VI&quot;. This control is a combobox labelled &quot;Frame buffer swap on:&quot;</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1940"/>
        <source>VI origin change (faster, few game issues)</source>
        <extracomment>&quot;VI&quot; stands for &quot;vertical interrupt&quot;. This control is a combobox labelled &quot;Frame buffer swap on:&quot;</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1945"/>
        <source>Color buffer change (fastest, some game issues)</source>
        <extracomment>This control is a combobox labelled &quot;Frame buffer swap on:&quot;</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2047"/>
        <source>Never (fastest, many game issues)</source>
        <extracomment>This is a combobox option with the label &quot;Copy video card frame buffer to N64 memory&quot;.</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2052"/>
        <source>Synchronous (slowest, fewest game issues)</source>
        <extracomment>This is a combobox option with the label &quot;Copy video card frame buffer to N64 memory&quot;.</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2057"/>
        <source>Asynchronous (fast, few game issues)</source>
        <extracomment>This is a combobox option with the label &quot;Copy video card frame buffer to N64 memory&quot;.</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2103"/>
        <source>Never (fastest, most game issues)</source>
        <extracomment>This is a combobox option with the label &quot;Copy video card frame buffer to N64 memory&quot;.</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2108"/>
        <source>From VRAM (slow, some game issues)</source>
        <extracomment>This is a combobox option with the label &quot;Copy video card frame buffer to N64 memory&quot;.</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2113"/>
        <source>In software (fast, fewest game issues)</source>
        <extracomment>This is a combobox option with the label &quot;Copy video card frame buffer to N64 memory&quot;.</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2167"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Enable force depth buffer clear. A hack. Necessary for Eikou no Saint Andrews.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;off, except for Eikou no Saint Andrews&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2170"/>
        <source>Force depth buffer clear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2187"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option is required for some Reshade depth dependant effects, such as SSAO and depth of field.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;only if you use Reshade and need depth dependant effects&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2190"/>
        <source>Copy depth to main depth buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2238"/>
        <source>Texture enhancement</source>
        <translation type="unfinished">テクスチャ拡張</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2284"/>
        <source>Filter:</source>
        <translation>フィルタ :</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2354"/>
        <source>Enhancement:</source>
        <translation>拡張:</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2491"/>
        <location filename="configDialog.ui" line="2686"/>
        <source>Use file storage instead of memory cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2505"/>
        <source>Use texture pack</source>
        <translation>テクスチャパックを使用する</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2565"/>
        <source>Texture pack path:</source>
        <translation>テクスチャパックのパス:</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2578"/>
        <source>Texture cache path:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2600"/>
        <location filename="configDialog.ui" line="2616"/>
        <location filename="configDialog.ui" line="2632"/>
        <source>...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2648"/>
        <source>Texture dump path:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2751"/>
        <source>Size of memory cache for enhanced textures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2814"/>
        <source>Compress texture cache</source>
        <translation>テクスチャキャッシュを圧縮</translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="17"/>
        <location filename="ConfigDialog.cpp" line="900"/>
        <source>GLideN64 Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="57"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;All the resolutions that your video card/monitor supports should be displayed.&lt;/p&gt;&lt;p&gt;[Recommended:&lt;span style=&quot; font-style:italic;&quot;&gt; Maximum resolution for your monitor unless performance becomes an issue&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="206"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Vertical sync, or VSync, can improve the image by syncing the game&apos;s frame rate to your monitor&apos;s refresh rate. This prevents image tearing, but may cause performance problems.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Usually off, on if you have image tearing problems&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="209"/>
        <source>Enable VSync</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="151"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This setting adjusts the aspect ratio of the video output. All N64 games support &lt;span style=&quot; font-weight:600;&quot;&gt;4:3&lt;/span&gt;. Some games support &lt;span style=&quot; font-weight:600;&quot;&gt;16:9&lt;/span&gt; within game settings. Use &lt;span style=&quot; font-weight:600;&quot;&gt;Stretch&lt;/span&gt; to fill the screen without pillar or letterboxing.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Try to adjust game to fit&lt;/span&gt; tries to adjust the viewing space to fit without stretching. Many games work well adjusted, but some don&apos;t.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="172"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Aspect ratio:&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="180"/>
        <source>4:3 (recommended)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="195"/>
        <source>Try to adjust game to fit</source>
        <extracomment>This option activates a widescreen hack. The language &quot;Try to&quot; implies it may it may not work and the language &quot;adjust game&quot; implies the emulation will be inaccurate.</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="465"/>
        <location filename="configDialog.ui" line="679"/>
        <source>Off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="573"/>
        <location filename="configDialog.ui" line="793"/>
        <source>High</source>
        <extracomment>The highest of a set of numbers</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="807"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Bilinear filtering&lt;/span&gt;: Textures will use standard PC-style bilinear filtering.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;N64-style 3 point&lt;/span&gt;: Textures will be filtered more like the N64. The result is less smooth but more accurate.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="836"/>
        <source>Bilinear filtering:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="845"/>
        <source>Standard</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="852"/>
        <source>N64-style 3 point</source>
        <extracomment>3 point bilinear filtering as used by the Nintendo 64.</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2764"/>
        <source> MB</source>
        <extracomment>Megabytes</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="901"/>
        <source>Screenshot format:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="914"/>
        <source>JPEG</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="953"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The selected language will display after this window is closed and reopened.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1123"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The N64 uses a unique method of mip-mapping that&apos;s difficult to reproduce correctly on PCs. When checked, this option emulates N64-accurate mip-mapping. When unchecked, some games have sharper distant textures.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Checked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1126"/>
        <source>Enable N64-style mip-mapping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1136"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;In N64 games lighting is calculated per vertex. This option enables Phong shading, which provides smoother and more realistic lighting.&lt;br/&gt;&lt;br/&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Your preference&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <oldsource>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option emulates effects that use random color input. Checking this option may cause rare performance problems.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Checked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1139"/>
        <source>Enable per-pixel lighting (better quality, HLE only)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1146"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Use persistent storage for compiled shader programs.&lt;br/&gt;Each game uses a set of combiners. A combiner is an equation that defines how to build output color from various color inputs. GLideN64 translates shaders, and compiles shader programs on the fly. Shaders are large and complex. If the game uses several new combiners, compiling new shaders will take time and result in stuttering. When this option is checked, these shaders are saved so they&apos;re not recompiled the next time you run the game.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Checked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1149"/>
        <source>Store compiled shaders for performance (recommended)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1167"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option sets the output render buffer size. By default, the render buffer is set to the &lt;span style=&quot; font-weight:600;&quot;&gt;Same as output resolution&lt;/span&gt;, but you can set it to the &lt;span style=&quot; font-weight:600;&quot;&gt;Original N64 resolution&lt;/span&gt; or a &lt;span style=&quot; font-weight:600;&quot;&gt;Multiple of N64 resolution&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1179"/>
        <source>Same as output resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1189"/>
        <source>Original N64 resolution (most accurate)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1206"/>
        <source>Multiple of N64 resolution:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1494"/>
        <location filename="configDialog.ui" line="1504"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When checked, 2D elements are rendered at the N64s resolution before copying them to output. This usually eliminates display issues with 2D elements, but it can be slow. This option uses heuristics to detect adjacent 2D elements that doesn&apos;t work for every game.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Checked, unless you have performance problems&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1542"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;In some games the coordinates for parts of 2D elements are not aligned: there is a half-pixel split between adjacent elements. When rendering at the N64&apos;s original resolution it isn&apos;t visible, but when the image is scaled up it results in black lines. This option attempts to connect these 2D elements.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;For adjacent 2D elements&lt;/span&gt;: Apply the correction only for polygons that need it. Select this option for games that have issues with black lines.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Always&lt;/span&gt;: Apply the correction for every 2D element. Select this option when &lt;span style=&quot; font-weight:600;&quot;&gt;For adjacent 2D elements&lt;/span&gt; doesn&apos;t help.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Never&lt;/span&gt;: Don&apos;t attempt to correct black lines between 2D elements.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Game dependent, mostly Never&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1563"/>
        <source>Fix black lines between 2D elements:</source>
        <extracomment>2D elements, formerly labelled texrects, are usually used for title screens or HUDs</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1579"/>
        <source>For adjacent 2D elements</source>
        <extracomment>The label for this control is &quot;Fix black lines between 2D elements&quot;</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1589"/>
        <source>Always</source>
        <extracomment>The label for this control is &quot;Fix black lines between 2D elements&quot;</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1599"/>
        <source>Never</source>
        <extracomment>The label for this control is &quot;Fix black lines between 2D elements&quot;</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1760"/>
        <source>Frame buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1889"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When checked, the auxiliary color buffer is copied to N64 memory right after rendering to it is finished. This option helps to correctly emulate frame buffer effects in some games. This option may noticeably reduce performance.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Usually unchecked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1892"/>
        <source>Copy auxiliary color buffers to N64 memory (sometimes slow, fewer game issues)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1906"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option controls how often GLideN64 updates the main frame buffer.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Vertical interrupt&lt;/span&gt;: Update the buffer as quickly as possible, every vertical interrupt per second (50 for PAL and 60 for NTSC games). This option is the most compatible. Use this mode unless you have performance problems.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;VI origin change&lt;/span&gt;: The VI origin changes corresponding to the game&apos;s actual FPS. N64 games typically run between 20 to 30 FPS so the buffers swap less often than the first mode. This mode does not work for games that use single buffering, which is rare.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Color buffer change&lt;/span&gt;: This option checks to see if the frame buffer has been changed. There are some games where this doesn&apos;t work.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1927"/>
        <source>Frame buffer swap on:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1974"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Some emulators do a poor job of detecting when to read/write frame buffers. You can disable emulator help to let GLideN64 read/write frame buffers itself.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Usually unchecked, but for some games/emulators it may be faster checked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1977"/>
        <source>Use emulator help to read/write frame buffers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1987"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When checked, the color buffer will be read in chunks. Otherwise, the buffer will be read in its entirety. If a game needs to read the entire buffer, selecting this option may be slow.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Usually unchecked, because the color buffer is usually read entirely&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1990"/>
        <source>Read color buffer in 4 KB chunks</source>
        <extracomment>&quot;Read&quot; refers to &quot;Use emulator help to read/write frame buffers&quot;</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1997"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When checked, the depth buffer will be read in chunks. Otherwise the buffer will be read in its entirety. If a game needs to read the entire buffer, selecting this option may be slow.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Checked, because the depth buffer is not often read entirely&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2000"/>
        <source>Read depth buffer in 4 KB chunks</source>
        <extracomment>&quot;Read&quot; refers to &quot;Use emulator help to read/write frame buffers&quot;</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2018"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;In some games GLideN64 can&apos;t detect when the game uses the frame buffer. With these options, you can have GLideN64 copy each frame of your video card&apos;s frame buffer to N64 memory.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Never&lt;/span&gt;: Disable copying buffers from video card.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Synchronous&lt;/span&gt;: Effects are detected for all games, but it can be slow. Use for games where &lt;span style=&quot; font-weight:600;&quot;&gt;Asynchronous&lt;/span&gt; doesn&apos;t work.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Asynchronous&lt;/span&gt;: Effects are detected for most games.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Usually Asynchronous&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2039"/>
        <source>Copy video card frame buffer to N64 memory:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2068"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;The depth buffer is used to emulate some effects (e.g. coronas):&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;Never&lt;/span&gt;: Depth buffer is disabled.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;From VRAM&lt;/span&gt;: Your video card&apos;s depth buffer is copied to N64 memory each frame, which can be slow on some games.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600;&quot;&gt;In software&lt;/span&gt;: Generally faster than copying from VRAM, but the result can be imperfect.&lt;/p&gt;&lt;p&gt;[Recommended:&lt;span style=&quot; font-style:italic;&quot;&gt; In software&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2095"/>
        <source>Copy video card depth buffer to N64 memory:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2177"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When this option is checked, the frame buffer is rendered directly to the screen. This prevents some graphic problems but may cause slowdowns or visual quality problems.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Usually unchecked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2180"/>
        <source>Render N64 frame buffer to output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2247"/>
        <source>N64 textures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2266"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This filter smooths or sharpens textures. There are four smoothing filters and two sharpening filters. The higher the number, the stronger the effect. Performance may be affected depending on the game and/or your PC.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Your preference&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2292"/>
        <location filename="configDialog.ui" line="2362"/>
        <source>None</source>
        <extracomment>No texture filtering
----------
No texture enhancement</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2297"/>
        <source>Smooth filtering 1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2302"/>
        <source>Smooth filtering 2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2307"/>
        <source>Smooth filtering 3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2312"/>
        <source>Smooth filtering 4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2317"/>
        <source>Sharp filtering 1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2322"/>
        <source>Sharp filtering 2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2333"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;There are 12 distinct filters to select. Depending on which filter, they may cause performance problems.&lt;/p&gt;&lt;p&gt;When &lt;span style=&quot; font-weight:600;&quot;&gt;Store&lt;/span&gt; is selected, textures are saved to the cache as-is. This improves performance in games that load many textures. Uncheck &lt;span style=&quot; font-weight:600;&quot;&gt;Disable for backgrounds&lt;/span&gt; for the best performance.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Your preference&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2367"/>
        <source>Store</source>
        <extracomment>Store texture enhancement</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2471"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option enables a pre-processing step that reduces posterization issues on enhanced textures.&lt;/p&gt;&lt;p&gt;[Recommended:&lt;span style=&quot; font-style:italic;&quot;&gt; Checked for xBRZ&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2474"/>
        <source>Decrease color banding (recommended for xBRZ enhancement)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2481"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option skips texture enhancements for long, narrow textures that are usually used for backgrounds. This may save texture memory and improve performance.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Checked, unless Enhancement is set to Store&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2484"/>
        <source>Disable for backgrounds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2724"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Enhanced and filtered textures can be cached to improve performance. This option adjusts how much memory is dedicated to the texture cache. This can improve performance if there are many requests for the same texture, which is usually the case. Normally 128 MB should be more than enough, but the best option is different for each game. Super Mario 64 may not need more than 32 MB, but Conker&apos;s Bad Fur Day can take advantage of 256 MB+. Adjust accordingly if you are having performance problems. Setting this option to 0 disables the cache.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;PC and game dependent&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2535"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select path to the folder with texture packs.&lt;br/&gt;Default: Plugin/hires_texture&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2663"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;When this option is cleared, textures will be loaded as they are when using Rice Video: transparencies either on or off. When this option is selected, GlideN64 will check how the texture&apos;s alpha channel was designed and will select the most appropriate format. This gives texture pack designers freedom to use semi-transparent textures.&lt;/p&gt;&lt;p&gt;Clear this option for older or poorly designed texture packs.&lt;/p&gt;&lt;p&gt;[Recommended:&lt;span style=&quot; font-style:italic;&quot;&gt; Texture pack dependent&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2666"/>
        <source>Use full transparencies (recommended for newer texture packs)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2673"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option emulates a palette CRC calculation bug in Rice Video. If you have problems loading textures, try checking or unchecking this option.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Mostly unchecked, checked for old texture packs&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2676"/>
        <source>Alternative CRC calculation (for old Rice Video packs)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2798"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option saves all previously loaded and enhanced textures to your PC. When the game is next launched, textures don&apos;t have to be recreated, causing smoother performance.&lt;/p&gt;&lt;p&gt;When using texture packs, loading packs will take only a few seconds when the game is launched as opposed to the 5–60 seconds that loading usually takes. However, if you change the texture pack you&apos;ll have to manually delete the texture cache. Saved cache files are saved to a folder called Cache within the plugins folder.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Checked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2801"/>
        <source>Save enhanced texture cache to hard disk</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2811"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Textures will be compressed so more textures can be held in the cache. The compression ratio varies per texture, but the compression is typically 1/5 of the original size.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Checked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2821"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option halves the space used by textures in the texture cache and video card memory to improve performance. When reducing the color, GLideN64 tries to perserve the original quality as much as possible. On most textures it&apos;s hardly noticeable, but some textures, like skies, can look noticeably worse.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Unchecked&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2824"/>
        <source>Convert textures to 16 bpp</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="1249"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Some N64 games use gamma correction to brighten the image. When the frame buffer is enabled, gamma correction is applied automatically for all games that use it on the N64. You can use your own gamma correction instead with this option. The default level, used on the N64, is 2.&lt;/p&gt;&lt;p&gt;[Recommended: &lt;span style=&quot; font-style:italic;&quot;&gt;Unchecked; 2.00&lt;/span&gt;]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2847"/>
        <location filename="ConfigDialog.cpp" line="858"/>
        <source>OSD</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2850"/>
        <source>On-screen display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2863"/>
        <source>Position</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3209"/>
        <source>Top left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3011"/>
        <source>Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3313"/>
        <source>Top right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3118"/>
        <source>Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2907"/>
        <source>Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="2959"/>
        <source>Bottom left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3261"/>
        <source>Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3063"/>
        <source>Bottom right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3506"/>
        <source>Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3572"/>
        <source>Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="configDialog.ui" line="3364"/>
        <source>Display percentage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="331"/>
        <source>Save and Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="332"/>
        <source>Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="333"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="334"/>
        <location filename="ConfigDialog.cpp" line="688"/>
        <location filename="ConfigDialog.cpp" line="693"/>
        <source>Restore Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="393"/>
        <location filename="ConfigDialog.cpp" line="906"/>
        <location filename="ConfigDialog.cpp" line="914"/>
        <source>New...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="567"/>
        <source>The texture pack folder is missing. Please change the folder or turn off texture packs.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="582"/>
        <source>The texture pack cache folder is missing. Please change the folder or turn off texture packs.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="597"/>
        <source>The texture dump folder is missing. Please change the folder or turn off dumping texture packs.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="689"/>
        <source>Are you sure you want to reset all settings to default?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="694"/>
        <location filename="ConfigDialog.cpp" line="973"/>
        <source>Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="910"/>
        <location filename="ConfigDialog.cpp" line="915"/>
        <location filename="ConfigDialog.cpp" line="921"/>
        <location filename="ConfigDialog.cpp" line="927"/>
        <source>New Profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="910"/>
        <source>New profile name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="916"/>
        <source>New settings profiles cannot be called &quot;New...&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="922"/>
        <source>Please type a name for your new settings profile.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="928"/>
        <source>This settings profile already exists.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="967"/>
        <source>Are you sure you want to remove the settings profile &quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="968"/>
        <source>&quot;?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ConfigDialog.cpp" line="969"/>
        <source>Remove Profile</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="FullscreenResolutions_windows.cpp" line="82"/>
        <source> Hz</source>
        <extracomment>Abbreviation for Hertz; include a leading space if appropriate</extracomment>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
