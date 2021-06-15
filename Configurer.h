#pragma once

#define DEFAULT_BLUE

#include "HelperStructs.h"

#include <vector>

class UIFrame;

class Configurer {
private:
  // WskaŸnik do g³ównego okna programu.
  UIFrame* m_mainFrame;
  std::vector<Segment> m_prismData;
  // Prêdkoœæ p³aszczyzny.
  float m_v;
  // Aktualnie wybrana p³aszczyzna: 1 -- prost. do OX, 2 -- prost do OY, 3 -- prost do OZ.
  int m_planeId;
  // Po³o¿enie p³aszczyzny wzd³u¿ osi (0: œrodek uk³adu).
  float m_pos;
  // Rozmiary panelu, na którym wyœwietlany jest przekrój.
  int m_sizeX, m_sizeY;
  // Po³o¿enie panelu, na którym wyœwietlany jest przekrój.
  int m_posX, m_posY;
  // Geometria za³adowana - dopisane
  bool m_geoLoaded;
  // Zapisywanie do pliku
  bool m_saveToFile;
public:
  Configurer(UIFrame* parent) :
    m_v(0), m_planeId(1),
    m_pos(0),
    m_posX(0), m_posY(0),
    m_sizeX(0), m_sizeY(0),
    m_geoLoaded(false),
    m_saveToFile(false),
    m_mainFrame(parent)
  {}

  void setV(float _v) { m_v = _v; }
  float getV() const { return m_v; }
  void setPlaneId(int _pId) { m_planeId = _pId; }
  float getPlaneId() const { return m_planeId; }
  void setPos(float _pos) { m_pos = _pos; }
  float getPos() const { return m_pos; }
  void setSizeX(int _sizeX) { m_sizeX = _sizeX;  }
  int getSizeX() const { return m_sizeX; }
  void setSizeY(int _sizeY) { m_sizeY = _sizeY; }
  int getSizeY() const { return m_sizeY; }
  void setPosX(int _posX) { m_posX = _posX; }
  int getPosX() { return m_posX; }
  void setPosY(int _posY) { m_posY = _posY; }
  int getPosY() { return m_posY; }
  //Dopisane
  void setGeoLoadedStatus(bool _geoLoaded) { m_geoLoaded = _geoLoaded; }
  bool isGeoLoaded() { return m_geoLoaded; }
  void setSaveToFile(bool _saveToFile) { m_saveToFile = _saveToFile; }
  bool isSaveToFile() { return m_saveToFile; }
  //Koniec dopisanych
  std::vector<Segment>& getData() { return m_prismData; }
  void setData(std::vector<Segment>& _data) { m_prismData = _data; }

  void startAnimation();

  void loadPrism(const char* filename);
  void saveAnimation(const char* filename);
};