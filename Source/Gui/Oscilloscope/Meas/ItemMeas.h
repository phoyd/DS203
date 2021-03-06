#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_MEAS_ITEMMEAS_H
#define DSO_GUI_OSCILLOSCOPE_MEAS_ITEMMEAS_H
#include <Source/Core/Controls.h>
#include <Source/Core/Settings.h>

class CItemMeas : public CWndMenuItem {
 public:
  CSettings::Measure *m_pMeas;
  ui16 m_color;

 public:
  virtual void Create(CSettings::Measure *pMeas, CWnd *pParent) {
    _ASSERT(pMeas);
    m_pMeas = pMeas;
    m_color = Settings.CH1.u16Color;
    CWndMenuItem::Create(NULL, RGB565(000000), 2, pParent);
    CWndMenuItem::SetColorPtr(&m_color);
  }

  virtual void OnPaint() {
    bool bEnabled = m_pMeas->Enabled == CSettings::Measure::_On;
    ui16 clr = bEnabled ? RGB565(000000) : RGB565(808080);
    ui16 clr2 = RGB565(404040);

    switch (m_pMeas->Source) {
      case CSettings::Measure::_CH1:
        m_color = Settings.CH1.u16Color;
        break;
      case CSettings::Measure::_CH2:
        m_color = Settings.CH2.u16Color;
        break;
      case CSettings::Measure::_Math:
        m_color = Settings.Math.uiColor;
        break;
    }

    CWndMenuItem::OnPaint();

    int x = m_rcClient.left + 10 + MarginLeft;
    int y = m_rcClient.top;
    BIOS::LCD::Print(x, y, clr, RGBTRANS,
                     CSettings::Measure::ppszTextType[(int)m_pMeas->Type]);

    if (bEnabled) {
      y += 16;
      char str[32];
      const char *suffix =
          CSettings::Measure::ppszTextSuffix[(int)m_pMeas->Type];
      float fValue = m_pMeas->fValue;
      if (fValue < 0) {
        x += BIOS::LCD::Draw(x, y, clr, RGBTRANS, CShapes::minus);
        fValue = -fValue;
      } else
        x += 6;

      BIOS::DBG::sprintf(str, "%3f", fValue);

      while (CUtils::StrLen(str) + CUtils::StrLen(suffix) > 7)
        str[CUtils::StrLen(str) - 1] = 0;

      x += BIOS::LCD::Print(x, y, clr, RGBTRANS, str);
      x += 4;
      if (suffix && *suffix) BIOS::LCD::Print(x, y, clr2, RGBTRANS, suffix);
    }
  }

  virtual void OnKey(ui16 nKey) {
    _ASSERT(m_pMeas);
    if (nKey & BIOS::KEY::KeyEnter) {
      SendMessage(m_pParent, ToWord('m', 'c'), m_pMeas);
      return;
    }
    CWnd::OnKey(nKey);
  }
};

#endif
