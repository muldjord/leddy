
#ifndef SPICONN_H
#define SPICONN_H

#include <stdint.h>
#include <QObject>
#include <QImage>
#include <QTimer>
#include <QEventLoop>

class SPIConn : public QObject
{
  Q_OBJECT

public:
  SPIConn(QByteArray device, uint32_t speed, uint8_t mode, uint8_t bits,
	  int brightness = 100, int rotation = 180);
  ~SPIConn();
  bool init();
  bool refresh();
  void clear(const QColor color = QColor(Qt::black));
  void setFromImage(const QImage image);
  void setPixel(const int x, const int y, const QColor color);
  void drawText(const int x, const int y,
		const QString text, const int spacing = 0, const QColor color = QColor(Qt::white));
  
private:
  QTimer limitTimer;
  QEventLoop limiter;
  QImage buffer = QImage(16, 16, QImage::Format_RGB888);
  QByteArray device = "";
  uint8_t bits = 8;
  uint8_t mode = 0;
  uint32_t speed = 9000000;
  uint16_t delay = 0;
  int rotation = 180;
  int brightness = 100;
  
  bool isOpen = false;
  int fd;
};

#endif // SPICONN_H
