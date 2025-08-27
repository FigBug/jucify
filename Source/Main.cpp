#include <JuceHeader.h>

#include <regex>
#include <fstream>
#include <iostream>
#include <unordered_set>

// List of JUCE classes you want to prefix
std::vector<std::string> juceClasses = {
	"String", "Array", "OwnedArray", "ValueTree", "KeyPressMappingSet", "ActionListener", "SparseSet", "FileSearchPath",
	"MidiKeyboardComponent", "RecentlyOpenedFilesList", "DirectoryIterator", "FileFilter", "TimeSliceThread", "FileTreeComponent",
    "DirectoryContentsDisplayComponent", "int8", "OutputStream", "TooltipWindow", "MouseInactivityDetector", "AudioData",
    "CPlusPlusCodeTokeniser", "KeyListener", "MouseInactivityDetector", "CodeTokeniser", "LocalisedStrings", "InputSource",
    "DirectoryContentsList", "sendNotificationAsync", "TimeSliceClient", "InputSource", "ScopedUnlock", "FileBrowserComponent",
    "LV2PluginFormat", "VST3PluginFormat", "VSTPluginFormat", "SettableTooltipClient", "sendNotificationAsync",
    "AudioDeviceSelectorComponent", "FlexBox", "FlexItem", "FileBrowserComponent", "FileChooserDialogBox", "ComponentMovementWatcher",
    "ChildProcessCoordinator", "FileSearchPathListComponent", "TextInputTarget", "ResizableBorderComponent", "AudioThumbnailBase",
    "ChildProcessWorker", "GenericAudioProcessorEditor", "Atomic", "negativeAwareModulo",
	"AudioBuffer", "AudioProcessor", "MidiMessage", "ResizableCornerComponent", "ScaledImage", "ImageConvolutionKernel",
	"Graphics", "Colour", "Component", "Button", "DeletedAtShutdown", "Time", "int64", "CaretComponent", "KnownPluginList",
	"ComponentBoundsConstrainer", "sendNotification", "SparseSet", "RowDragSource", "roundToIntAccurate", "CallOutBox",
	"CodeEditorComponent", "CodeDocument", "MidiKeyboardState", "DrawableButton", "FilePreviewComponent", "SortedSet",
	"Label", "Slider", "File", "FileInputStream", "FileOutputStream", "Image", "DynamicObject", "BufferedInputStream",
	"KeyMappingEditorComponent", "DrawableImage", "ImageComponent", "AudioProcessorValueTreeStateParameterAttributes",
	"StringArray", "MidiMessage", "TextEditor", "Colours", "Colour", "Font", "Justification", "var", "OggVorbisAudioFormat",
	"MemoryOutputStream", "MouseEvent", "MidiInput", "MidiOutput", "Range", "Timer", "URL", "Changebroadcaster", "ScaledImage",
    "DrawableShape", "JUCEApplicationBase", "MD5", "DefaultElementComparator", "AudioFormatWriter",
	"AlertWindow", "ComponentPeer", "ScopedPointer", "WeakReference", "AsyncUpdater", "MemoryBlock", "ChangeListener", "MidiKeyboardStateListener",
	"Identifier", "JSON", "NamedValueSet", "MidiOutput", "CriticalSection", "NormalisableRange", "uint8", "CharacterFunctions", "nsStringToJuce",
	"PNGImageFormat", "ScopedLock", "StringPairArray", "MidiInputCallback", "HashMap", "IPAddress", "XmlElement", "ResizableWindow",
	"MessageManager", "ChangeBroadcaster", "SystemStats", "int32", "jmax", "jmin", "jlimit", "TextButton", "GroupComponent", "ListenerList",
	"MathConstants", "LookAndFeel_V3", "uint32", "ImageFileFormat", "uint16", "int16", "TextLayout", "StretchableLayoutManager", "StreamingSocket",
	"WaitableEvent", "Thread", "MultiTimer", "JUCEApplication", "dontSendNotification", "OwnedArray", "Uuid", "PopupMenu", "RectanglePlacement",
	"Viewport", "Typeface", "DatagramSocket", "InterprocessConnection", "MenuBarModel", "TreeView", "TabbedComponent", "KeyPress", "Rectangle",
	"TreeViewItem", "ignoreUnused", "MemoryInputStream", "Path", "ChildProcess", "ImageCache", "ModifierKeys", "Desktop", "AttributedString",
	"ToggleButton", "ToggleButton", "WebBrowserComponent", "GlyphArrangement", "ComboBox", "MouseCursor", "PropertiesFile", "ApplicationProperties",
	"LookAndFeel", "InterProcessLock", "DocumentWindow", "SharedResourcePointer", "RelativeTime", "ListBox", "ListBoxModel", "ByteOrder", "BorderSize", "addBytesToPointer",
	"numElementsInArray", "DialogWindow", "BorderSize", "addBytesToPointer", "XmlDocument", "FileChooser", "AffineTransform", "TableListBoxModel",
	"TableListBox", "TabbedButtonBar", "SystemClipboard", "CharPointer_UTF8", "UnitTest", "ScopedValueSetter", "roundDoubleToInt", "BigInteger",
	"juce_wchar", "UnitTestRunner", "Logger", "MessageBoxOptions", "FileLogger", "ReferenceCountedArray", "Value", "ReferenceCountedObject",
	"ComponentListener", "CallbackMessage", "MouseListener", "ColourGradient", "PathStrokeType", "RectangleList", "MidiBuffer", "AudioProcessorEditor",
	"PluginDescription", "Point", "AudioThumbnail", "Line", "ShapeButton", "HeapBlock", "ThreadWithProgressWindow", "isPositiveAndNotGreaterThan",
	"isPositiveAndBelow", "roundToInt", "DrawablePath", "PixelARGB", "Drawable", "FontOptions", "CachedValue", "ReferenceCountedObjectPtr",
	"InputStream", "Process", "MouseWheelDetails", "TemporaryFile", "StringRef", "ZipFile", "LookAndFeel_V4", "CommandID", "BubbleComponent",
	"ProgressBar", "TableHeaderComponent", "ScrollBar", "AudioSampleBuffer", "AudioFormatReader", "LagrangeInterpolator", "TabBarButton",
	"MessageManagerLock", "Base64", "UndoManager", "RangedDirectoryIterator", "AudioPlayHead", "OptionalScopedPointer", "ApplicationCommandTarget",
	"ApplicationCommandInfo", "FillType", "DragAndDropTarget", "ValueTreePropertyWithDefault", "jmap", "SpinLock", "AudioPluginInstance", "AudioProcessorListener",
	"TopLevelWindow", "AudioPluginFormatManager", "AudioChannelSet", "FloatVectorOperations", "WebInputStream", "Optional", "AudioProcessorValueTreeState",
	"IIRCoefficients", "AbstractFifo", "Decibels", "Random", "GridItem", "Grid", "UndoableAction", "NamedPipe", "PluginListComponent", "AudioPluginFormat",
	"RSAKey", "FilenameComponentListener", "FilenameComponentListener", "DrawableRectangle", "FileDragAndDropTarget", "TooltipClient", "ModalComponentManager",
	"AudioProcessorParameter", "FocusChangeListener", "translate", "ColourSelector", "TracktionMarketplaceStatus", "DragAndDropContainer", "Result",
	"ActionBroadcaster", "OnlineUnlockStatus", "AiffAudioFormat", "WavAudioFormat", "MouseInputSource", "MidiMessageSequence", "newLine",
	"PerformanceCounter", "readUnaligned", "BubbleMessageComponent", "FilenameComponent", "AudioFormatManager", "AudioFormatManager", "AudioThumbnailCache",
	"ThreadPoolJob", "MidiFile", "ModalCallbackFunction", "HyperlinkButton", "ApplicationCommandManager", "JavascriptEngine", "FilenameComponent",
	"ChoicePropertyComponent"
};

// checks whether a character is valid inside a C++ identifier (ASCII version)
static bool isIdentifierChar (char c) noexcept
{
	return (c == '_') || (c >= '0' && c <= '9') ||
		   (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static std::string addJuceNamespaceToText (const std::string& input, const std::vector<std::string>& juceClasses)
{
	std::string s = input;

	for (const auto& cls : juceClasses)
	{
		size_t start = 0;
		while (true)
		{
			size_t pos = s.find (cls, start);
			if (pos == std::string::npos)
				break;

			size_t clsLen = cls.length();

			// Whole identifier check
			bool leftOk  = (pos == 0) || !isIdentifierChar (s[pos - 1]);
			bool rightOk = (pos + clsLen >= s.size()) || !isIdentifierChar (s[pos + clsLen]);

			if (leftOk && rightOk)
			{
				bool alreadyPrefixed =  (pos >= 2 && s.substr(pos - 2, 2) == "::") ||
										(pos >= 1 && s.substr(pos - 1, 1) == "->") ||
										(pos >= 1 && s.substr(pos - 1, 1) == "\"") ||
										(pos >= 1 && s.substr(pos - 1, 1) == "\\") ||
										(pos >= 1 && s.substr(pos - 1, 1) == ".")  ||
										(pos >= 1 && s.substr(pos - 1, 1) == "/");
				if (! alreadyPrefixed)
				{
					s.insert (pos, "juce::");
					start = pos + 6 + clsLen;
					continue;
				}
			}

			start = pos + clsLen;
		}
	}

	return s;
}

static void processFile(const juce::File& file, const std::vector<std::string>& juceClasses)
{
	if (! file.existsAsFile())
	{
		std::cerr << "Skipping (not a file): " << file.getFullPathName() << '\n';
		return;
	}

	// Read as raw bytes to preserve line endings and spacing
	std::ifstream in (file.getFullPathName().toStdString(), std::ios::binary);
	if (! in)
	{
		std::cerr << "Failed to read: " << file.getFullPathName() << '\n';
		return;
	}

	std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();

	if (content.find ("jucify: ignore") != std::string::npos)
	{
		std::cout << "Ignored: " << file.getFullPathName() << '\n';
		return;
	}

	std::string updated = addJuceNamespaceToText(content, juceClasses);

	if (updated != content)
	{
		std::ofstream out (file.getFullPathName().toStdString(), std::ios::binary | std::ios::trunc);
		if (! out)
		{
			std::cerr << "Failed to write: " << file.getFullPathName() << '\n';
			return;
		}
		out.write (updated.data(), static_cast<std::streamsize> (updated.size()));
		out.close();

		std::cout << "Updated: " << file.getFullPathName() << '\n';
	}
	else
	{
		std::cout << "No changes: " << file.getFullPathName() << '\n';
	}
}

//==============================================================================
int main (int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: jucify --recursive --exclude=a,b,c <path-to-cpp-file> [more-files...]\n";
		return 1;
	}

	juce::Array<juce::File> files;
	bool recursive = false;
	juce::StringArray excludeTokens;

	for (auto i = 1; i < argc; i++)
	{
		auto param = juce::String (argv[i]);

		if (param.startsWith ("--"))
		{
			if (param == "--recursive")
			{
				recursive = true;
			}
			else if (param.startsWith ("--exclude"))
			{
				auto tokens = juce::StringArray::fromTokens (param.fromFirstOccurrenceOf ("=", false, false), ",", "");
				excludeTokens.addArray (tokens);
			}
		}
		else
		{
			files.add (param);
		}
	}

	auto exclude = [&] (const juce::File& f)
	{
		auto path = f.getFullPathName();
		for (auto t : excludeTokens)
			if (path.contains (t))
				return true;

		return false;
	};

	// Process each file
	for (auto file : files)
	{
		if (file.existsAsFile())
		{
			processFile(file, juceClasses);
		}
		else if (file.isDirectory())
		{
			for (auto f : file.findChildFiles (juce::File::findFiles, recursive))
			{
				if (exclude (f))
					continue;

				if (f.hasFileExtension ("cpp;c;h;hpp;mm"))
					processFile (f, juceClasses);
			}
		}
		else
		{
			std::cerr << "File not found: " << file.getFullPathName() << '\n';
		}
	}

	return 0;
}
