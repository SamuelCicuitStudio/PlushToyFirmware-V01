---

## Dependency Flow Chart

The following diagram illustrates the dependency flow among the various classes used in the project. Each class is structured to show its relationships and the direction of calls, making it easier to understand the architecture of the application.

```
                                 ┌───────────────────────┐
                                 │      Config.h         │
                                 └───────────────────────┘
                                           │
                                           ▼
                                 ┌───────────────────────┐
                                 │   Configuration.h     │
                                 └───────────────────────┘
                                           │
                                           ▼
                               ┌───────────┼──────────────────┐
                               │           │                  │
                               ▼           ▼                  ▼
                     ┌─────────────┐   ┌─────────────┐   ┌─────────────┐
                     │ GPIOManager │   │ PowerManager│   │ WiFiManager │
                     └─────────────┘   └─────────────┘   └─────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │   OtaManager     │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │   I2SManager     │
                                   └──────────────────┘
                                           │
                               ┌───────────┼──────────────────┐
                               │           │                  │
                               ▼           ▼                  ▼
                     ┌────────────┐   ┌──────────────┐   ┌──────────────┐
                     │ MicManager │   │SpeakerManager│   │ SDCardManager│
                     └────────────┘   └──────────────┘   └──────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │ SPIFlashManager  │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │  DownloadStory   │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │   StoryTelling   │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │   WavFileReader  │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────────┐
                                   │   WavFileWriter  │
                                   └──────────────────┘
                                           │
                                           ▼
                               ┌───────────┼───────────┐
                               │                       │
                               ▼                       ▼
                     ┌─────────────────┐   ┌─────────────┐
                     │SpeechToText     │   │TextToSpeech │
                     │ Manager         │   │ Manager     │
                     └─────────────────┘   └─────────────┘
                               │                   │
                               │                   │
                               │                   │
                               ▼                   ▼
                      ┌──────────────────┐   ┌─────────────────┐
                      │ RandomBehavior   │   │   GameManager   │
                      │   Manager        │   └─────────────────┘
                      └──────────────────┘           │
                                                     ▼
                                          ┌───────────────────────┐
                                          │   VocalCommandManager │
                                          └───────────────────────┘
```

### Explanation
- **Classes Overview**: This flowchart displays the architecture of the project, illustrating how different components are interconnected.
- **Dependencies**: The arrows represent calls between classes, indicating which classes invoke methods of other classes. Understanding these relationships is crucial for navigating and maintaining the codebase.

---
