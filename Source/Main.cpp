#include <JuceHeader.h>

#include <regex>
#include <fstream>
#include <iostream>
#include <unordered_set>

// List of JUCE classes you want to prefix
std::vector<std::string> juceClasses = {
	"String", "Array", "OwnedArray", "ValueTree",
	"AudioBuffer", "AudioProcessor", "MidiMessage",
	"Graphics", "Colour", "Component", "Button", "DeletedAtShutdown", "Time", "int64",
	"Label", "Slider", "File", "FileInputStream", "FileOutputStream", "Image", "DynamicObject",
	"StringArray", "MidiMessage", "TextEditor", "Colours", "Colour", "Font", "Justification", "var",
	"MemoryOutputStream", "MouseEvent", "MidiInput", "MidiOutput", "Range", "Timer", "URL", "Changebroadcaster",
	"AlertWindow", "ComponentPeer", "ScopedPointer", "WeakReference", "AsyncUpdater", "MemoryBlock", "ChangeListener",
	"Identifier", "JSON", "NamedValueSet", "MidiOutput", "CriticalSection", "NormalisableRange", "uint8", "CharacterFunctions",
	"PNGImageFormat", "ScopedLock", "StringPairArray", "MidiInputCallback", "HashMap", "IPAddress", "XmlElement", "ResizableWindow",
	"MessageManager", "ChangeBroadcaster", "SystemStats", "int32", "jmax", "jmin", "jlimit", "TextButton", "GroupComponent", "ListenerList",
	"MathConstants", "LookAndFeel_V3", "uint32", "ImageFileFormat", "uint16", "int16", "TextLayout", "StretchableLayoutManager", "StreamingSocket",
	"WaitableEvent", "Thread", "MultiTimer", "JUCEApplication", "dontSendNotification", "OwnedArray", "Uuid", "PopupMenu", "RectanglePlacement",
	"Viewport", "Typeface", "DatagramSocket", "InterprocessConnection", "MenuBarModel", "TreeView", "TabbedComponent", "KeyPress", "Rectangle",
	"TreeViewItem", "ignoreUnused", "MemoryInputStream", "Path", "ChildProcess", "ImageCache", "ModifierKeys", "Desktop", "AttributedString",
	"ToggleButton", "ToggleButton", "WebBrowserComponent", "GlyphArrangement", "ComboBox", "MouseCursor", "PropertiesFile", "ApplicationProperties",
	"LookAndFeel", "InterProcessLock", "DocumentWindow", "SharedResourcePointer", "RelativeTime", "ListBox", "ListBoxModel", "ByteOrder", "BorderSize", "addBytesToPointer"
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
	"IIRCoefficients", "AbstractFifo", "Decibels", "Random", "GridItem", "Grid", "UndoableAction"
};

// checks whether a character is valid inside a C++ identifier (ASCII version)
static bool isIdentifierChar(char c) noexcept
{
	return (c == '_') || (c >= '0' && c <= '9') ||
		   (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static std::string addJuceNamespaceToText(const std::string& input,
										  const std::vector<std::string>& juceClasses)
{
	std::string s = input;

	for (const auto& cls : juceClasses)
	{
		size_t start = 0;
		while (true)
		{
			size_t pos = s.find(cls, start);
			if (pos == std::string::npos)
				break;

			size_t clsLen = cls.length();

			// Whole identifier check
			bool leftOk  = (pos == 0) || !isIdentifierChar(s[pos - 1]);
			bool rightOk = (pos + clsLen >= s.size()) || !isIdentifierChar(s[pos + clsLen]);

			if (leftOk && rightOk)
			{
				bool alreadyPrefixed = (pos >= 2 && s.substr(pos - 2, 2) == "::") || (pos >= 1 && s.substr(pos - 1, 1) == "\"");
				if (!alreadyPrefixed)
				{
					s.insert(pos, "juce::");
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
	if (!file.existsAsFile())
	{
		std::cerr << "Skipping (not a file): " << file.getFullPathName() << '\n';
		return;
	}

	// Read as raw bytes to preserve line endings and spacing
	std::ifstream in(file.getFullPathName().toStdString(), std::ios::binary);
	if (!in)
	{
		std::cerr << "Failed to read: " << file.getFullPathName() << '\n';
		return;
	}

	std::string content((std::istreambuf_iterator<char>(in)),
						 std::istreambuf_iterator<char>());
	in.close();


	if (content.find("jucify: ignore") != std::string::npos)
	{
		std::cout << "Ignored: " << file.getFullPathName() << '\n';
		return;
	}

	std::string updated = addJuceNamespaceToText(content, juceClasses);

	if (updated != content)
	{
		std::ofstream out(file.getFullPathName().toStdString(),
						  std::ios::binary | std::ios::trunc);
		if (!out)
		{
			std::cerr << "Failed to write: " << file.getFullPathName() << '\n';
			return;
		}
		out.write(updated.data(), static_cast<std::streamsize>(updated.size()));
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
		std::cout << "Usage: " << argv[0] << " <path-to-cpp-file> [more-files...]\n";
		return 1;
	}

	// Process each file
	for (int i = 1; i < argc; ++i)
	{
		juce::File file(argv[i]);
		if (file.existsAsFile())
		{
			processFile(file, juceClasses);
		}
		else if (file.isDirectory())
		{
			for (auto f : file.findChildFiles(juce::File::findFiles, true))
			{
				if (f.getFullPathName().contains("3rd_party"))
					continue;

				if (f.hasFileExtension ("cpp;c;h;hpp;mm"))
					processFile (f, juceClasses);
			}
		}
	}

	return 0;
}
