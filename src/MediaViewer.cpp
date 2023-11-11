
#include "MediaViewer.h"
#include "BinaryData.h"
#include <juce_events/juce_events.h>

MediaViewer *MediaViewer::sfInstance = nullptr;

class MyDocumentWindow : public juce::DocumentWindow
{
  public:
    MyDocumentWindow() : DocumentWindow("GP Media Viewer", juce::Colours::black, DocumentWindow::allButtons, true)
    {
    }
    virtual ~MyDocumentWindow()
    {
    }
    virtual void closeButtonPressed() override
    {
        setVisible(false);
    };
};

MediaViewer::MediaViewer()
{
    image.reset(new ImageComponent());
    addAndMakeVisible(image.get());

    defaultImage = ImageFileFormat::loadFrom(BinaryData::DefaultImage_png, (size_t)BinaryData::DefaultImage_pngSize);
    image->setImage(defaultImage);

    video.reset(new VideoComponent(true));
    addChildComponent(video.get());
    //video->onPlaybackStopped([](){MediaViewer::playbackStopped();});
    video->onPlaybackStopped = [](void) { MediaViewer::playbackStopped();};

    //VideoComponent vc;
    //vc.onPlaybackStarted( this {DBG("\n\n video has started !\n"); };
    //vc.onPlaybackStarted = [this](void) { MediaViewer::playbackStopped();};

    setSize(720, 480);
    // auto result = video->load(File("/Users/kieran/Movies/2022-03-30 20-51-29.mp4"));

    fWindow.reset(new MyDocumentWindow());
    fWindow->setContentNonOwned(this, true);
    fWindow->setResizable(true, true);
    fWindow->setUsingNativeTitleBar(true);

#if JUCE_WINDOWS
    fWindow->getPeer()->setIcon(getWindowIcon());
#endif
}

MediaViewer::~MediaViewer()
{
    image = nullptr;
    video = nullptr;
}

void MediaViewer::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
}

void MediaViewer::resized()
{
    image->setBounds(0, 0, getWidth(), getHeight());
    video->setBounds(0, 0, getWidth(), getHeight());
    // video->setBounds (getLocalBounds().reduced (10));
}

void MediaViewer::initialize()
{
    juce::MessageManager::getInstance()->callAsync([]() {
        if (sfInstance == nullptr)
        {
            sfInstance = new MediaViewer();
            sfInstance->fWindow->setTopLeftPosition(100, 100);
        }

        jassert(sfInstance != nullptr);
        sfInstance->fWindow->setVisible(false);
    });
}

void MediaViewer::finalize()
{
    if (sfInstance != nullptr)
    {
        delete sfInstance;
        sfInstance = nullptr;
    }
}

void MediaViewer::showWindow()
{
    sfInstance->fWindow->setVisible(true);
    sfInstance->fWindow->toFront(true);
}

void MediaViewer::hideWindow()
{
    sfInstance->fWindow->setVisible(false);
}

void MediaViewer::displayMedia(String path)
{
    Image imageFile = ImageFileFormat::loadFrom(File(path));
    if (imageFile.isValid())
    {
        if (sfInstance->video->isVideoOpen())
            sfInstance->video->stop();
        sfInstance->video->setVisible(false);
        sfInstance->image->setVisible(true);
        sfInstance->image->setImage(imageFile);
    }
    else
    {

        auto result = sfInstance->video->load(File(path));
        if (result.wasOk())
        {
            sfInstance->image->setVisible(false);
            sfInstance->video->setVisible(true);
            if (sfInstance->video->isVideoOpen())
                sfInstance->video->play();
        }
        else
        {
            if (sfInstance->video->isVideoOpen())
                sfInstance->video->stop();
            sfInstance->video->setVisible(false);
            sfInstance->image->setVisible(true);
            sfInstance->image->setImage(sfInstance->defaultImage);
        }
    }
}
 void  MediaViewer::playbackStopped() {
    if (sfInstance->video->isVideoOpen()) {
        /*
    if (sfInstance->video->getPlayPosition() == sfInstance->video->getVideoDuration()) {
        sfInstance->video->play();
    }
    */
   sfInstance->video->play();
    }
 }

Image MediaViewer::getWindowIcon()
{
    Image img;
    String imageBase64 = WINDOW_ICON;
    MemoryOutputStream mo;
    auto result = Base64::convertFromBase64(mo, imageBase64);
    if (result)
    {
        img = ImageFileFormat::loadFrom(mo.getData(), mo.getDataSize());
    }
    return img;
}