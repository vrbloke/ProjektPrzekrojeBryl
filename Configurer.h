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
  // Wspó³rzêdne wektora prêdkoœci.
  float m_vx, m_vy, m_vz;
  // Wspó³rzêdne wektora normalnego p³aszczyzny.
  float m_px, m_py, m_pz;
  // Po³o¿enie p³aszczyzny wzd³u¿ kierunku wektora normalnego (0: œrodek uk³adu).
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
    m_vx(0), m_vy(0), m_vz(0),
    m_px(0), m_py(0), m_pz(0),
    m_pos(0),
    m_posX(0), m_posY(0),
    m_sizeX(0), m_sizeY(0),
    m_geoLoaded(false),
    m_saveToFile(false),
    m_mainFrame(parent)
  {}

  void setVx(float _vx) { m_vx = _vx; }
  float getVx() const { return m_vx; }
  void setVy(float _vy) { m_vy = _vy; }
  float getVy() const  { return m_vy; }
  void setVz(float _vz) { m_vz = _vz; }
  float getVz() const { return m_vz; }
  void setPx(float _px) { m_px = _px; normalizeP(); }
  float getPx() const { return m_px; }
  void setPy(float _py) { m_py = _py; normalizeP();  }
  float getPy() const { return m_py; }
  void setPz(float _pz) { m_pz = _pz; normalizeP();  }
  float getPz() const { return m_pz; }
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

  void normalizeP() {
    Vector4 normal = Normalize(Vector4(m_px, m_py, m_pz));
    //m_px = normal.GetX();
    //m_py = normal.GetY();
    //m_pz = normal.GetZ();
  }

  void startAnimation();

  void loadPrism(const char* filename);
  void saveAnimation(const char* filename);
};