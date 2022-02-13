QSlider slider;

// QT3:
slider = new QSlider(8, 128, 1, 6, Qt::Vertical, 0, "volume");

// QT4:
slider = new QSlider(Qt::Vertical);
slider->setRange(8, 128);
slider->setValue(6);
slider->setObjectName("volume");