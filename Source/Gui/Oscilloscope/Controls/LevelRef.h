#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_CONTROLS_LEVELREF_H
#define DSO_GUI_OSCILLOSCOPE_CONTROLS_LEVELREF_H
#include <Source/Core/Settings.h>
#include <Source/Framework/Wnd.h>
#include <Source/Gui/Oscilloscope/Controls/GraphBase.h>

class CWndLevelReferences : public CWnd {
 public:
  virtual void Create(CWnd *pParent, ui16 dwFlags) {
    CWnd::Create("CWndLevelReferences", dwFlags | CWnd::WsNoActivate,
                 CRect(0, 22, 11, 22 + CWndGraph::DivsY * CWndGraph::BlkY),
                 pParent);
  }

  virtual void OnPaint() {
    BIOS::LCD::Bar(m_rcClient, RGB565(000000));
    // if ( Settings.Trig.Sync != CSettings::Trigger::_None )
    {
      ui16 y = Settings.Trig.nLevel;
      y = (y * (CWndGraph::DivsY * CWndGraph::BlkY)) >> 8;
      BIOS::LCD::Draw(m_rcClient.left, m_rcClient.bottom - y - 5,
                      RGB565(606060), RGBTRANS, CShapes::trig_base);
    }
    if (Settings.CH2.Enabled) {
      // ui16 y = Settings.CH2.u16Position;
      // y = CSettings::GetZero(y);
      CSettings::Calibrator::FastCalc fastCalc;
      Settings.CH2Calib.Prepare(&Settings.CH2, fastCalc);
      ui16 y = Settings.CH2Calib.GetZero(fastCalc);
      y = (y * (CWndGraph::DivsY * CWndGraph::BlkY)) >> 8;

      BIOS::LCD::Draw(m_rcClient.left, m_rcClient.bottom - y - 5,
                      Settings.CH2.u16Color, RGBTRANS, CShapes::chb_base);
    }
    if (Settings.CH1.Enabled) {
      // si16 y = Settings.CH1.u16Position;
      // y = CSettings::GetZero(y);
      CSettings::Calibrator::FastCalc fastCalc;
      Settings.CH1Calib.Prepare(&Settings.CH1, fastCalc);
      si16 y = Settings.CH1Calib.GetZero(fastCalc);

      y = (y * (CWndGraph::DivsY * CWndGraph::BlkY)) >> 8;
      int nScrY = m_rcClient.bottom - y - 5;
      if (nScrY >= 0 && nScrY + 7 < BIOS::LCD::LcdHeight) {
        BIOS::LCD::Draw(m_rcClient.left, nScrY, Settings.CH1.u16Color, RGBTRANS,
                        CShapes::cha_base);
      }
    }
  }
};

class CWndLevelReferencesMath : public CWnd {
 public:
  virtual void Create(CWnd *pParent, ui16 dwFlags) {
    CWnd::Create("CWndLevelReferencesMath", dwFlags | CWnd::WsNoActivate,
                 CRect(0, 22, 11, 22 + CWndGraph::DivsY * CWndGraph::BlkY),
                 pParent);
  }

  virtual void OnPaint() {
    if (Settings.Math.Type != CSettings::MathOperator::_Off) {
      si16 y = Settings.Math.Position;

      y = (y * (CWndGraph::DivsY * CWndGraph::BlkY)) >> 8;
      int nScrY = m_rcClient.bottom - y - 5;
      if (nScrY >= 0 && nScrY + 7 < BIOS::LCD::LcdHeight) {
        BIOS::LCD::Draw(m_rcClient.left, nScrY, Settings.Math.uiColor, RGBTRANS,
                        CShapes::chm_base);
      }
    }
  }
};
#endif
