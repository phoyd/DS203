
#include "MainMenu.h"
#include <Source/Core/Design.h>
#include <Source/Core/Shapes.h>
#include <Source/Gui/MainWnd.h>

const CWndModuleSelector::TMenuBlockStruct *CWndModuleSelector::GetLayout() {
  static const CWndModuleSelector::TMenuBlockStruct arrLayout[9] = {
      // CWnd, Label, color, target sent to Toolbar.cpp
      {&m_itmOscilloscope, "Oscillo\nscope", RGB565(ffffff), "Oscilloscope",
       (PVOID)iconOscilloscope},
      {&m_itmSpectrum, "Spectrum\nanalyser", RGB565(ffffff), "Spectrum",
       (PVOID)iconSpectrum},
      {&m_itmGenerator, "Signal\ngenerator", RGB565(ffffff), "Generator",
       (PVOID)iconGenerator},

      {&m_itmSettings, "Settings", RGB565(ffffff), "Settings",
       (PVOID)iconSettings},
      {&m_itmUser, "User\napplications", RGB565(ffffff), "User app",
       (PVOID)iconUser},
      {&m_itmAbout, "About", RGB565(ffffff), "About", (PVOID)iconAbout},

      {&m_itmApp2, "Application\nslot 2", RGB565(808080), "APP2", NULL},
      {&m_itmApp3, "Application\nslot 3", RGB565(808080), "APP3", NULL},
      {&m_itmApp4, "Application\nslot 4", RGB565(808080), "APP4", NULL},
      //		{ NULL,					NULL,
      // RGB565(808080), NULL,			NULL },
  };

  return arrLayout;
}

/*virtual*/ void CWndModuleSelector::Create(CWnd *pParent, ui16 dwFlags) {
  CWnd::Create("CWndModuleSelector", dwFlags, CRect(0, 16, 400, 240), pParent);

#define _BORDER 20
#define _SPACING 8
#define _LEFT(x)              \
  m_rcClient.left + _BORDER + \
      (m_rcClient.Width() - _BORDER * 2 - _SPACING) * (x) / 3
#define _RIGHT(x) _LEFT(x + 1) - _SPACING
#define _TOP(y)              \
  m_rcClient.top + _BORDER + \
      (m_rcClient.Height() - _BORDER * 2 - _SPACING) * (y) / 3
#define _BOTTOM(y) _TOP(y + 1) - _SPACING
#define _ITEM(x, y) CRect(_LEFT(x), _TOP(y), _RIGHT(x), _BOTTOM(y))

  CRect rcItem;
  const CWndModuleSelector::TMenuBlockStruct *arrLayout = GetLayout();

  int nIndex = 0;
  for (int y = 0; y < 3; y++)
    for (int x = 0; x < 3; x++, nIndex++) {
      const CWndModuleSelector::TMenuBlockStruct *pItem = &arrLayout[nIndex];
      if (pItem->m_pWnd) {
        CRect rcItem = _ITEM(x, y);
        pItem->m_pWnd->Create(pItem->m_strLabel, pItem->m_clr, rcItem, this,
                              pItem->m_icon);
      }
    }
}

/*virtual*/ void CWndModuleSelector::OnPaint() {
  BIOS::LCD::Bar(m_rcClient.left, m_rcClient.top, m_rcClient.right,
                 m_rcClient.bottom, RGB565(0020ff));
  CWnd::OnPaint();
}

/*virtual*/ void CWndModuleSelector::OnKey(ui16 nKey) {
  if (nKey & BIOS::KEY::KeyEnter) {
    const CWndModuleSelector::TMenuBlockStruct *arrLayout = GetLayout();
    int nId = _GetItemId(GetFocus());
    const char *strTarget = arrLayout[nId].m_strTarget;

    if (strcmp(strTarget, "APP2") == 0) {
      if (BIOS::SYS::IdentifyApplication(BIOS::SYS::EApp2))
        BIOS::SYS::Execute(BIOS::SYS::EApp2);
      else
        MainWnd.m_wndMessage.Show(this, "Info", "Application not installed",
                                  RGB565(FFFF00));
    } else if (strcmp(strTarget, "APP3") == 0) {
      if (BIOS::SYS::IdentifyApplication(BIOS::SYS::EApp3))
        BIOS::SYS::Execute(BIOS::SYS::EApp3);
      else
        MainWnd.m_wndMessage.Show(this, "Info", "Application not installed",
                                  RGB565(FFFF00));
    } else if (strcmp(strTarget, "APP4") == 0) {
      if (BIOS::SYS::IdentifyApplication(BIOS::SYS::EApp4))
        BIOS::SYS::Execute(BIOS::SYS::EApp4);
      else
        MainWnd.m_wndMessage.Show(this, "Info", "Application not installed",
                                  RGB565(FFFF00));
    } else if (strTarget) {
      SendMessage(&MainWnd.m_wndToolBar, ToWord('g', 'o'), strTarget);
    } else {
      MainWnd.m_wndMessage.Show(this, "Info", "Sorry, not implemented",
                                RGB565(FFFF00));
    }
    return;
  }
  if (nKey & (BIOS::KEY::KeyLeft | BIOS::KEY::KeyRight | BIOS::KEY::KeyUp |
              BIOS::KEY::KeyDown)) {
    CWnd *pCurrent = GetFocus();
    int nCurrentId = _GetItemId(pCurrent);
    _ASSERT(nCurrentId >= 0 && nCurrentId <= 9);
    int nNewId = nCurrentId;

    if (nKey & BIOS::KEY::KeyLeft) nNewId--;
    if (nKey & BIOS::KEY::KeyRight) nNewId++;
    if (nKey & BIOS::KEY::KeyUp) nNewId -= 3;
    if (nKey & BIOS::KEY::KeyDown) nNewId += 3;
    if ((nNewId < 0) || (nNewId >= 9)) {
      MainWnd.m_wndToolBar.SetFocus();
      pCurrent->Invalidate();
      MainWnd.m_wndToolBar.Invalidate();
      return;
    }

    CWnd *pNew = NULL;
    if (nNewId != nCurrentId) pNew = _GetWindowById(nNewId);
    if (pNew) {
      pNew->SetFocus();
      pCurrent->Invalidate();
      pNew->Invalidate();
    }
    return;
  }

  CWnd::OnKey(nKey);
}

int CWndModuleSelector::_GetItemId(CWnd *pWnd) {
  const CWndModuleSelector::TMenuBlockStruct *arrLayout = GetLayout();

  for (int i = 0; i < 9; i++)
    if (arrLayout[i].m_pWnd == pWnd) return i;
  return -1;
}

CWnd *CWndModuleSelector::_GetWindowById(int nId) {
  const CWndModuleSelector::TMenuBlockStruct *arrLayout = GetLayout();
  _ASSERT(nId >= 0 && nId <= 9);
  return arrLayout[nId].m_pWnd;
}

LINKERSECTION(".extra")
/*static*/ const unsigned char CWndModuleSelector::iconSettings[] = {
    0x47, 0x42, 0x29, 0x2b, 0x48, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x00, 0x30, 0x00, 0x38, 0xa0, 0x41, 0xe0, 0x00, 0x60, 0x23, 0x40,
    0x14, 0x80, 0x01, 0x00, 0x0e, 0x20, 0x02, 0x40, 0x07, 0xe0, 0x00, 0x00,
    0xfa, 0x1f, 0x18, 0xa3, 0xc1, 0xab, 0x82, 0xf1, 0x8a, 0x2c, 0x0d, 0x5f,
    0x13, 0xd4, 0xe1, 0xcb, 0xd1, 0xf1, 0xba, 0x5e, 0x1c, 0x8b, 0x1f, 0x23,
    0x1c, 0x5e, 0x1c, 0xbb, 0x1f, 0x4b, 0xce, 0xc9, 0x3e, 0x1b, 0x31, 0xf1,
    0x1a, 0x3e, 0x59, 0x5c, 0xec, 0xb9, 0x1f, 0x0b, 0x6e, 0x3c, 0x51, 0x3c,
    0x1f, 0x32, 0x45, 0xa5, 0xe1, 0xa2, 0x7f, 0x5b, 0xce, 0xc6, 0x91, 0xe0,
    0xc1, 0xe9, 0xa4, 0x21, 0x59, 0xec, 0x0c, 0x1e, 0x59, 0x6c, 0xec, 0xb5,
    0xf1, 0x2b, 0x5e, 0x3a, 0x54, 0x28, 0x1f, 0x1d, 0xa5, 0xf3, 0x24, 0x5c,
    0x4e, 0x19, 0x14, 0xf5, 0xbc, 0xec, 0x57, 0x1e, 0x2c, 0x09, 0x1e, 0x17,
    0x51, 0x12, 0x50, 0x91, 0xe2, 0xa0, 0xc1, 0xe5, 0x96, 0xce, 0xc8, 0x2f,
    0x22, 0x4c, 0x4e, 0x2a, 0x42, 0x4f, 0x33, 0xae, 0xa1, 0xf0, 0xc1, 0xe1,
    0xa3, 0x4f, 0x0b, 0x5e, 0x2c, 0x51, 0x1f, 0x5b, 0xce, 0xc6, 0x91, 0xe2,
    0xc0, 0xa1, 0xe0, 0x91, 0x04, 0x42, 0xf1, 0x51, 0x00, 0x91, 0xe2, 0x90,
    0xc1, 0xe9, 0x76, 0xce, 0xcb, 0xf1, 0x5c, 0x5e, 0x1c, 0xb2, 0xf1, 0x39,
    0x2e, 0x0c, 0x1f, 0x0a, 0x3e, 0x49, 0x3f, 0x8c, 0x6e, 0x4c, 0x51, 0xbc,
    0x1e, 0x15, 0x71, 0xe2, 0xc0, 0xa1, 0xe0, 0x91, 0x02, 0x51, 0x32, 0xf2,
    0x31, 0x51, 0x00, 0x91, 0xe2, 0x90, 0xc1, 0xe6, 0x96, 0xec, 0xb1, 0xa8,
    0x1e, 0x2c, 0xac, 0x4e, 0x0d, 0x1f, 0x11, 0xc8, 0x2e, 0x4a, 0x8b, 0x79,
    0x1e, 0x2c, 0x0a, 0x1e, 0x09, 0x10, 0x25, 0x13, 0x6f, 0x23, 0x15, 0x10,
    0x09, 0x1e, 0x7a, 0x0c, 0xec, 0x54, 0xc9, 0x2e, 0x19, 0x21, 0xf2, 0x34,
    0xa9, 0x2e, 0x0c, 0x14, 0x2a, 0x6a, 0x1e, 0x09, 0x10, 0x25, 0x13, 0xa1,
    0xf2, 0x31, 0x51, 0x00, 0x91, 0xe3, 0x95, 0x4c, 0x92, 0xe2, 0xa5, 0x13,
    0xf4, 0x31, 0x59, 0xc8, 0x2e, 0x4c, 0x4b, 0xc9, 0x10, 0x25, 0x13, 0xe1,
    0xf2, 0x31, 0x51, 0x02, 0x41, 0xcc, 0x1e, 0x0c, 0x1c, 0x5a, 0x70, 0x51,
    0x36, 0xf1, 0x31, 0x15, 0x00, 0x20, 0x0c, 0xc1, 0xe5, 0xa4, 0x54, 0x13,
    0xa2, 0xf3, 0x32, 0x8c, 0xc1, 0xe1, 0xc6, 0x25, 0x04, 0x11, 0x03, 0xb1,
    0xf1, 0x32, 0x21, 0x24, 0x5c, 0xc1, 0xe1, 0xc8, 0xc2, 0xf1, 0x8a, 0xc1,
    0xe3, 0xc5, 0x42, 0x83, 0xf3, 0xb8, 0x6c, 0xc1, 0xe1, 0xc8, 0x82, 0xf1,
    0x8c, 0xc1, 0xe5, 0xc5, 0x4c, 0xeb, 0xe2, 0xf1, 0xbc, 0x1e, 0x3a, 0x45,
    0xcc, 0x1e, 0x1a, 0x2c, 0x1f, 0x18, 0xcc, 0x1e, 0x1c, 0x61, 0x40, 0x92,
    0xe1, 0xcb, 0xa2, 0xf1, 0xbc, 0x2e, 0x69, 0x05, 0x14, 0x6c, 0xc1, 0xe1,
    0xc8, 0x81, 0xf1, 0x2c, 0xc1, 0xe2, 0xc5, 0x41, 0x20, 0x41, 0x00, 0x92,
    0xe1, 0xcb, 0xd1, 0xf1, 0xda, 0x3e, 0x09, 0x10, 0x75, 0x13, 0xf2, 0x45,
    0xcc, 0x1e, 0x1c, 0x24, 0xf1, 0x2a, 0xc1, 0xe3, 0xc6, 0x42, 0x2f, 0x23,
    0x15, 0x10, 0x09, 0x4e, 0x0c, 0x81, 0xf0, 0xd4, 0xe0, 0x91, 0x02, 0x51,
    0x34, 0xf3, 0x24, 0x6c, 0x5e, 0x4c, 0x70, 0x7c, 0x1e, 0x1d, 0x31, 0xf1,
    0x19, 0x1e, 0x06, 0x20, 0x06, 0x4e, 0x3c, 0x54, 0x26, 0xf4, 0x31, 0x50,
    0x75, 0xe0, 0xb5, 0xf0, 0xa5, 0xe3, 0x05, 0x13, 0x81, 0xf3, 0x24, 0x6c,
    0x2e, 0x0c, 0x10, 0x25, 0x45, 0x1e, 0x1a, 0x41, 0x21, 0x59, 0x1e, 0x48,
    0x32, 0x46, 0x2e, 0x3c, 0x64, 0x2a, 0x1f, 0x42, 0x40, 0x7c, 0x4e, 0x0d,
    0x2f, 0x1b, 0xc3, 0xe0, 0xa1, 0x02, 0x54, 0x3b, 0x1f, 0x32, 0x46, 0xc1,
    0xe4, 0xcb, 0x8b, 0xc1, 0xe6, 0x75, 0x12, 0x50, 0x96, 0xe3, 0xc5, 0x42,
    0xd1, 0xf1, 0x24, 0x20, 0x17, 0xc2, 0xe0, 0xc1, 0xf0, 0xd1, 0xe0, 0x92,
    0x03, 0x54, 0x23, 0xe1, 0xf3, 0x24, 0x6a, 0x4e, 0x09, 0x10, 0x44, 0x2f,
    0x15, 0x10, 0x07, 0x2a, 0x39, 0x54, 0x29, 0x2f, 0x32, 0x14, 0x51, 0x02,
    0x99, 0x11, 0xf1, 0x24, 0x10, 0x35, 0x42, 0x3b, 0x2f, 0x22, 0x45, 0x40,
    0x25, 0x13, 0x2f, 0x23, 0x14, 0x25, 0x24, 0x12, 0xe2, 0xf1, 0x31, 0x14,
    0x11, 0x32, 0xf0, 0x31, 0x20, 0x38, 0x3f, 0x13, 0x22, 0x11, 0x23, 0x2f,
};  // 557 bytes

LINKERSECTION(".extra")
/*static*/ const unsigned char CWndModuleSelector::iconOscilloscope[] = {
    0x47, 0x42, 0x33, 0x23, 0x48, 0x00, 0x38, 0x00, 0x28, 0x00, 0x18, 0x00,
    0x10, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xa0, 0x29, 0xa0,
    0x10, 0xc0, 0x23, 0x00, 0x1c, 0xa0, 0x0e, 0x40, 0x07, 0xc0, 0x00, 0x00,
    0x5f, 0x07, 0x26, 0x05, 0x16, 0x07, 0xb1, 0xf0, 0x75, 0x60, 0x78, 0x4f,
    0x97, 0x65, 0x49, 0xdc, 0xa6, 0x79, 0x1f, 0x06, 0x15, 0x64, 0xad, 0xca,
    0x67, 0x81, 0xf3, 0x76, 0xad, 0x2e, 0x49, 0x34, 0x57, 0x7f, 0x37, 0x65,
    0xd2, 0xe4, 0xa3, 0x46, 0x7d, 0x3f, 0x46, 0x53, 0x2c, 0x3e, 0x2b, 0x56,
    0x6f, 0x57, 0x64, 0x32, 0xd3, 0xe2, 0xb5, 0x66, 0xf6, 0x75, 0xad, 0xec,
    0x81, 0xe4, 0xb2, 0x35, 0x65, 0xf6, 0x76, 0x4d, 0xec, 0xb1, 0xe4, 0x92,
    0x45, 0x7b, 0x3f, 0x86, 0x43, 0x29, 0xed, 0x09, 0x1e, 0x23, 0x57, 0x4f,
    0x77, 0x64, 0x31, 0xce, 0xc1, 0x04, 0xee, 0xb4, 0x64, 0xf5, 0x75, 0x4d,
    0xec, 0x10, 0x0b, 0x1e, 0x48, 0x23, 0x57, 0x4f, 0x46, 0x4c, 0xed, 0x10,
    0x7c, 0xed, 0x12, 0x45, 0x79, 0x3f, 0x77, 0x53, 0x28, 0xde, 0x91, 0x05,
    0xbe, 0xc4, 0x57, 0x3f, 0x66, 0x43, 0x28, 0xed, 0x20, 0x08, 0x1e, 0x2a,
    0x57, 0x3f, 0x46, 0x4c, 0xed, 0x30, 0x6d, 0xec, 0x12, 0x46, 0x3f, 0x27,
    0x59, 0x1e, 0x09, 0x20, 0x6e, 0xeb, 0x13, 0x46, 0x6f, 0x07, 0x27, 0x1f,
    0xf1, 0x71, 0x66, 0x1a, 0x17, 0x74, 0xf6, 0x64, 0x21, 0xbe, 0xb3, 0x04,
    0xde, 0x94, 0x62, 0xf7, 0x75, 0x32, 0x1c, 0xec, 0x30, 0x5b, 0xed, 0x45,
    0x71, 0xf2, 0x64, 0x91, 0xe1, 0x90, 0x21, 0x79, 0xed, 0x92, 0x35, 0x71,
    0xf5, 0x75, 0x4d, 0xec, 0x10, 0x91, 0x1b, 0xed, 0x82, 0x35, 0x72, 0xf0,
    0x71, 0x62, 0xdc, 0xa1, 0x51, 0x66, 0x14, 0x59, 0xed, 0xa6, 0x72, 0xf4,
    0x64, 0x32, 0x81, 0xe0, 0x92, 0x16, 0x09, 0xed, 0x35, 0x71, 0xf7, 0x64,
    0x31, 0x9e, 0xd8, 0x12, 0xf1, 0x10, 0xce, 0x94, 0x6f, 0x75, 0x3d, 0xec,
    0x00, 0x10, 0x22, 0x26, 0xce, 0xb1, 0x24, 0x61, 0xf5, 0x64, 0xad, 0xe8,
    0x11, 0x92, 0x21, 0xde, 0x91, 0x34, 0x62, 0xfa, 0x65, 0xbe, 0xd9, 0x34,
    0x54, 0x31, 0x24, 0xbe, 0xb4, 0x61, 0xf7, 0x75, 0x42, 0x1d, 0xed, 0x32,
    0x11, 0x81, 0xec, 0xb4, 0x6f, 0x75, 0x32, 0x1d, 0xec, 0x21, 0x30, 0x21,
    0x18, 0x9e, 0xd3, 0x57, 0x64, 0xb1, 0xe3, 0x81, 0x23, 0x14, 0x73, 0x8e,
    0xd1, 0x23, 0x41, 0x77, 0x53, 0xbe, 0xb0, 0x12, 0x13, 0xf2, 0x9e, 0xc1,
    0x23, 0x57, 0xf7, 0x54, 0xce, 0xb0, 0x11, 0x23, 0x14, 0x33, 0x21, 0x91,
    0xe8, 0x34, 0x7f, 0x64, 0x32, 0x91, 0xe0, 0x92, 0x3d, 0x21, 0x0b, 0xed,
    0xa5, 0x76, 0x43, 0x19, 0x1e, 0x12, 0x31, 0x4a, 0x32, 0x10, 0xce, 0xc4,
    0x65, 0xa1, 0xe4, 0xb0, 0x12, 0x41, 0x5b, 0x32, 0xce, 0xc1, 0x24, 0x56,
    0x4a, 0x1e, 0x39, 0x12, 0x31, 0x4f, 0x38, 0xde, 0xb1, 0x34, 0x67, 0x64,
    0xbe, 0xd0, 0x01, 0x12, 0x31, 0x5f, 0x32, 0x10, 0xce, 0xc3, 0x57, 0x54,
    0x21, 0xbe, 0x0c, 0xb2, 0x35, 0x43, 0x21, 0x0d, 0xe9, 0x41, 0x57, 0x32,
    0x1c, 0xeb, 0x24, 0x16, 0x74, 0x32, 0x18, 0xde, 0xa1, 0x4c, 0x9e, 0xd0,
    0x12, 0x35, 0x76, 0x43, 0xb1, 0xec, 0x92, 0x34, 0x53, 0xce, 0xc0, 0x12,
    0x31, 0x5e, 0x42, 0xce, 0xd1, 0x23, 0x56, 0x5a, 0xde, 0x91, 0x11, 0x24,
    0x16, 0xd4, 0x32, 0x18, 0xde, 0x94, 0x54, 0x32, 0x11, 0xe2, 0x83, 0x41,
    0x61, 0x42, 0x11, 0x39, 0xec, 0x31, 0x47, 0x31, 0x8e, 0xd2, 0x35, 0x17,
    0xc5, 0x32, 0x10, 0xce, 0xc4, 0x3d, 0xe9, 0x11, 0xc3, 0x46, 0xf7, 0x53,
    0x2d, 0xec, 0x12, 0x13, 0xc9, 0xed, 0x81, 0x23, 0x46, 0x75, 0x32, 0x1e,
    0x2b, 0x13, 0x24, 0x7b, 0xec, 0x01, 0x23, 0x51, 0x7f, 0x54, 0x21, 0x0c,
    0xed, 0x34, 0x32, 0x1c, 0xec, 0x02, 0x13, 0x51, 0x77, 0x53, 0x21, 0x8d,
    0xe9, 0x13, 0x72, 0x1d, 0xeb, 0x24, 0x61, 0xf5, 0x74, 0x32, 0x19, 0x1d,
    0x13, 0x31, 0x95, 0x12, 0x34, 0x57, 0x1f, 0x76, 0x43, 0x8e, 0xd8, 0x11,
    0x22, 0xce, 0xc1, 0x1c, 0x24, 0x57, 0xf6, 0x42, 0x9e, 0xd8, 0x21, 0x30,
    0xa1, 0xe0, 0x81, 0x12, 0x34, 0x61, 0xf8, 0x75, 0x32, 0x10, 0xde, 0xc2,
    0x20, 0x91, 0xe3, 0x93, 0x46, 0x1f, 0x26, 0x43, 0x11, 0x0c, 0x1e, 0x92,
    0x21, 0xbe, 0xd8, 0x35, 0x72, 0xf2, 0x64, 0x32, 0x23, 0x34, 0x54, 0x23,
    0x24, 0x56, 0x2f, 0x67, 0x53, 0x2c, 0xec, 0x11, 0x09, 0x1e, 0x50, 0x12,
    0x34, 0x61, 0xf7, 0x75, 0x32, 0xde, 0xb1, 0x12, 0x8d, 0xeb, 0x01, 0x23,
    0x57, 0x2f, 0x26, 0x43, 0x11, 0x38, 0xde, 0x91, 0x16, 0xde, 0xc2, 0x45,
    0x71, 0xf8, 0x75, 0x32, 0x10, 0xde, 0xc1, 0x05, 0xee, 0xb2, 0x46, 0x3f,
    0x17, 0x61, 0x51, 0x44, 0x15, 0x06, 0x46, 0x07, 0x4f, 0xf6, 0x43, 0x2d,
    0xeb, 0x0d, 0xeb, 0x01, 0x24, 0x50, 0x70, 0xf1, 0xf3, 0x64, 0x38, 0x1e,
    0xa9, 0x0d, 0xed, 0x01, 0x23, 0x57, 0x3f, 0x67, 0x54, 0x21, 0x09, 0x1e,
    0x79, 0xce, 0xd2, 0x35, 0x73, 0xf5, 0x64, 0x31, 0x09, 0x1e, 0x7b, 0xde,
    0xc2, 0x35, 0x75, 0xf0, 0x74, 0x70, 0xfb, 0x1f, 0x36, 0x42, 0x93, 0xe6,
    0xc0, 0x12, 0x35, 0x73, 0xf4, 0x75, 0x32, 0xb1, 0xe8, 0xde, 0xc0, 0x12,
    0x34, 0x65, 0xf6, 0x75, 0x32, 0x10, 0x92, 0xe4, 0xd1, 0x34, 0x64, 0xf4,
    0x75, 0x42, 0x11, 0x00, 0xd1, 0xe4, 0xc2, 0x34, 0x69, 0x3f, 0x67, 0x54,
    0x28, 0xbc, 0x20, 0x32, 0x34, 0x65, 0xf4, 0x75, 0x32, 0x81, 0xb0, 0x01,
    0x04, 0x13, 0x46, 0x76, 0xf4, 0x75, 0x42, 0x12, 0x05, 0x12, 0x34, 0x67,
    0x5f, 0x47, 0x54, 0x21, 0x20, 0x41, 0x23, 0x46, 0xb3, 0xf4, 0x75, 0x43,
    0x21, 0x15, 0x22, 0x35, 0x67, 0x6f, 0x46, 0x54, 0x32, 0x11, 0x52, 0x23,
    0x46, 0x78, 0x1f, 0x17, 0x61, 0x40, 0x32, 0x33, 0x45, 0x67, 0x7f, 0x37,
    0x65, 0x43, 0x33, 0x45, 0x67, 0xc3, 0xf1, 0x76, 0x15, 0x04, 0x14, 0x25,
    0x67, 0x91, 0xf1, 0x76, 0x15, 0x04, 0x14, 0x25, 0x67, 0x5f,
};  // 511 bytes

LINKERSECTION(".extra")
/*static*/ const unsigned char CWndModuleSelector::iconSpectrum[] = {
    0x47, 0x42, 0x2a, 0x22, 0x60, 0x00, 0x00, 0x00, 0x28, 0x00, 0x88, 0x80,
    0x30, 0xc0, 0x72, 0x20, 0x53, 0xa0, 0x1a, 0xe0, 0x24, 0xa0, 0x00, 0xa0,
    0x1e, 0x60, 0x0d, 0xc0, 0x01, 0x20, 0x07, 0x40, 0x06, 0xc0, 0x07, 0xe0,
    0xd8, 0x11, 0x0c, 0x1b, 0x08, 0xb2, 0x10, 0xb1, 0xf0, 0x7e, 0x61, 0x12,
    0x51, 0xf0, 0xbb, 0x21, 0x0b, 0x1f, 0x15, 0x0d, 0x61, 0x10, 0x51, 0xf0,
    0xbd, 0x11, 0x0c, 0x2d, 0x27, 0x1b, 0x1f, 0x25, 0x0e, 0x1d, 0x0b, 0xc5,
    0x11, 0x2b, 0x1f, 0x2d, 0x05, 0x1f, 0x2b, 0x28, 0x2f, 0x0c, 0x71, 0x0c,
    0x2f, 0x26, 0x2b, 0x1f, 0x25, 0x0d, 0x1f, 0x28, 0x02, 0xe4, 0x10, 0x71,
    0xd2, 0x80, 0x81, 0xf2, 0xd0, 0x51, 0xf2, 0xb0, 0x62, 0xf0, 0xc3, 0x10,
    0xc1, 0xc1, 0x1c, 0x2f, 0x26, 0x0b, 0x1f, 0x25, 0x0d, 0x1f, 0x28, 0x08,
    0x1f, 0x27, 0x29, 0x2c, 0x01, 0x83, 0x10, 0xe1, 0xf2, 0xb2, 0x61, 0xf2,
    0x80, 0x81, 0xf2, 0xd0, 0x51, 0xf2, 0xb0, 0x62, 0xf1, 0x99, 0x1f, 0x1e,
    0xe1, 0xf1, 0x49, 0x2f, 0x26, 0x0b, 0x1f, 0x25, 0x0d, 0x1f, 0x28, 0x08,
    0x1f, 0x26, 0x0b, 0x1f, 0x1e, 0x2e, 0x11, 0x0c, 0x1c, 0x19, 0xc2, 0xb2,
    0x70, 0xa1, 0xf2, 0xb0, 0x61, 0xf2, 0x80, 0x81, 0xf2, 0xd0, 0x51, 0xf2,
    0xb0, 0x62, 0xf1, 0x40, 0x1f, 0x1e, 0xe1, 0xf1, 0x34, 0x2f, 0x26, 0x0b,
    0x1f, 0x25, 0x0d, 0x1f, 0x28, 0x08, 0x1f, 0x26, 0x0b, 0x1f, 0x2a, 0x37,
    0x2f, 0x1c, 0x81, 0xf1, 0xd9, 0x31, 0x12, 0xd1, 0xf1, 0x84, 0x2f, 0x27,
    0x3a, 0x1f, 0x2b, 0x06, 0x1f, 0x28, 0x08, 0x1f, 0x2d, 0x05, 0x1f, 0x2b,
    0x06, 0x2f, 0x14, 0x31, 0xf1, 0xee, 0x1f, 0x13, 0x42, 0xf2, 0x60, 0xb1,
    0xf2, 0x50, 0xd1, 0xf2, 0x80, 0x81, 0xf2, 0x60, 0xb1, 0xf2, 0xa3, 0x72,
    0xf1, 0x38, 0x1f, 0x2d, 0x02, 0x11, 0x24, 0x3d, 0x1f, 0x18, 0x32, 0xf2,
    0x73, 0xa1, 0xf2, 0xb0, 0x61, 0xf2, 0x80, 0x81, 0xf2, 0xd0, 0x51, 0xf2,
    0xb0, 0x62, 0xf1, 0x43, 0x1f, 0x1e, 0xe1, 0xf1, 0x34, 0x2f, 0x26, 0x0b,
    0x1f, 0x25, 0x0d, 0x1f, 0x28, 0x08, 0x1f, 0x26, 0x0b, 0x1f, 0x2a, 0x37,
    0x2f, 0x13, 0x81, 0xf2, 0xd3, 0x41, 0x12, 0x43, 0xd1, 0xf1, 0x83, 0x2f,
    0x27, 0x3a, 0x1f, 0x2b, 0x06, 0x1f, 0x28, 0x08, 0x1f, 0x2d, 0x05, 0x1f,
    0x2b, 0x06, 0x2f, 0x14, 0x31, 0xf1, 0xee, 0x1f, 0x13, 0x42, 0xf2, 0x60,
    0xb1, 0xf2, 0x50, 0xd1, 0xf2, 0x80, 0x81, 0xf2, 0x60, 0xb1, 0xf2, 0xa3,
    0x72, 0xf1, 0x38, 0x1f, 0x2d, 0x34, 0x11, 0x24, 0x3d, 0x1f, 0x18, 0x32,
    0xf2, 0x73, 0xa1, 0xf2, 0xb0, 0x61, 0xf2, 0x80, 0x81, 0xf2, 0xd0, 0x51,
    0xf2, 0xb0, 0x62, 0xf1, 0x43, 0x1f, 0x1e, 0xe1, 0xf1, 0x34, 0x2f, 0x26,
    0x0b, 0x1f, 0x25, 0x0d, 0x1f, 0x28, 0x08, 0x1f, 0x26, 0x0b, 0x1f, 0x2a,
    0x37, 0x2f, 0x13, 0x81, 0xf2, 0xd3, 0x21, 0x12, 0x23, 0xd1, 0xf1, 0x83,
    0x2f, 0x27, 0x3a, 0x1f, 0x2b, 0x06, 0x1f, 0x28, 0x08, 0x1f, 0x2d, 0x05,
    0x1f, 0x2b, 0x06, 0x2f, 0x14, 0x31, 0xf1, 0xee, 0x1f, 0x13, 0x42, 0xf2,
    0x60, 0xb1, 0xf2, 0x50, 0xd1, 0xf2, 0x80, 0x81, 0xf2, 0x60, 0xb1, 0xf2,
    0xa3, 0x72, 0xf1, 0x38, 0x1f, 0x2d, 0x32, 0x11, 0x22, 0x3d, 0x1f, 0x18,
    0x32, 0xf2, 0x73, 0xa1, 0xf2, 0xb0, 0x61, 0xf2, 0x80, 0x81, 0xf2, 0xd0,
    0x51, 0xf2, 0xb0, 0x62, 0xf1, 0x43, 0x1f, 0x1e, 0xe1, 0xf1, 0x34, 0x2f,
    0x26, 0x0b, 0x1f, 0x25, 0x0d, 0x1f, 0x28, 0x08, 0x1f, 0x26, 0x0b, 0x1f,
    0x2a, 0x37, 0x2f, 0x13, 0x81, 0xf2, 0xd3, 0x21, 0x12, 0x23, 0xd1, 0xf1,
    0x83, 0x2f, 0x27, 0x3a, 0x1f, 0x2b, 0x06, 0x1f, 0x28, 0x08, 0x1f, 0x2d,
    0x05, 0x1f, 0x2b, 0x06, 0x2f, 0x14, 0x31, 0xf1, 0xee, 0x1f, 0x13, 0x42,
    0xf2, 0x60, 0xb1, 0xf2, 0x50, 0xd1, 0xf2, 0x80, 0x81, 0xf2, 0x60, 0xb1,
    0xf2, 0xa3, 0x72, 0xf1, 0x38, 0x1f, 0x2d, 0x32, 0x11, 0x12, 0x32, 0xf1,
    0x83, 0x2f, 0x27, 0x3e, 0x1f, 0x2e, 0x06, 0x1f, 0x28, 0x0a, 0x2f, 0x10,
    0x51, 0xf2, 0xb0, 0x62, 0xf1, 0x43, 0x1f, 0x1d, 0xb1, 0xa2, 0x34, 0xb1,
    0xa2, 0x60, 0x81, 0xa2, 0x50, 0xb1, 0xa2, 0x60, 0x71, 0xa2, 0x50, 0x81,
    0xa2, 0x83, 0x72, 0xa1, 0x37, 0x2a, 0x13, 0x21, 0x10, 0x22, 0x31, 0x00,
    0x23, 0x20, 0x20, 0x13, 0x20, 0x20, 0x13, 0x20, 0x20, 0x23, 0x12, 0x01,
    0x32, 0x02, 0x02, 0x31, 0x22, 0x23, 0x12, 0x22, 0x04, 0x22, 0x03, 0x01,
    0x20, 0x01, 0x01, 0x22, 0x30, 0x12, 0x21, 0x01, 0x22, 0x30, 0x42, 0x20,
    0x30, 0x12, 0x30, 0x30, 0x22, 0x10, 0x21, 0x21, 0x11, 0x22, 0x11, 0x13,
    0x21, 0x11, 0x12, 0x11, 0x13, 0x21, 0x11, 0x22, 0x11, 0x12, 0x21, 0x11,
    0x22, 0x01,
};  // 662 bytes

LINKERSECTION(".extra")
/*static*/ const unsigned char CWndModuleSelector::iconGenerator[] = {
    0x47, 0x42, 0x20, 0x31, 0x48, 0x20, 0x30, 0x20, 0x20, 0x20, 0x10, 0x41,
    0x90, 0x20, 0x70, 0x20, 0x71, 0xa0, 0x4a, 0x60, 0x3b, 0xe0, 0x10, 0xe0,
    0x11, 0xa0, 0x0a, 0x60, 0x16, 0x00, 0x0b, 0xe0, 0x07, 0xc0, 0x00, 0x00,
    0xd8, 0x1f, 0x1b, 0xb5, 0xf2, 0xde, 0xac, 0x3f, 0x33, 0xae, 0xd4, 0xf3,
    0x3c, 0xe9, 0xa1, 0xf3, 0xec, 0x13, 0x3f, 0x4d, 0xe7, 0x13, 0x93, 0xf4,
    0x30, 0x6e, 0xd2, 0xf4, 0xbd, 0x8c, 0xe2, 0xd0, 0xf3, 0xf0, 0xa6, 0xe1,
    0xd3, 0x1f, 0x4d, 0xe6, 0x03, 0x93, 0xfc, 0x30, 0x6e, 0xdf, 0x31, 0x07,
    0x6c, 0xe1, 0x71, 0xb9, 0x3f, 0x19, 0xa1, 0x51, 0xec, 0x14, 0x85, 0x02,
    0xfd, 0xe6, 0x03, 0x93, 0xf7, 0x30, 0x6e, 0xdf, 0x31, 0x10, 0x24, 0xce,
    0x10, 0x02, 0x5f, 0x63, 0x23, 0x76, 0x51, 0x13, 0x6f, 0xbc, 0xe8, 0x73,
    0xf2, 0xf0, 0xb1, 0xc0, 0xe3, 0xe6, 0xcc, 0xd9, 0x05, 0x1b, 0x1f, 0x22,
    0x1d, 0x4e, 0x0c, 0x1c, 0x0e, 0x2e, 0x1c, 0xbf, 0x1f, 0x19, 0xd2, 0xe1,
    0xc8, 0x34, 0x05, 0x18, 0x0e, 0x1e, 0x1c, 0xaa, 0x1f, 0x0a, 0x2e, 0x1c,
    0x68, 0x14, 0x06, 0x2e, 0x1c, 0x9a, 0x1f, 0x13, 0xc1, 0xe2, 0x84, 0x51,
    0x00, 0x12, 0x11, 0x05, 0x24, 0x07, 0x2e, 0x0b, 0x7f, 0x0c, 0x1e, 0x06,
    0x14, 0x35, 0x02, 0x33, 0xf5, 0x32, 0x10, 0x48, 0x1e, 0x1b, 0x37, 0xf0,
    0x21, 0xe3, 0xc5, 0x12, 0x81, 0xf2, 0x31, 0x01, 0x40, 0x71, 0xe0, 0xb5,
    0xf2, 0xce, 0xc1, 0x44, 0x02, 0xfb, 0xa8, 0x1f, 0x23, 0x17, 0x1e, 0x18,
    0x24, 0xf6, 0x31, 0xce, 0x81, 0x38, 0x1f, 0x0d, 0x1e, 0x5c, 0xa2, 0x04,
    0x61, 0xe0, 0xb3, 0xf9, 0xde, 0xc4, 0x51, 0xfc, 0xec, 0x1e, 0x0b, 0x7f,
    0x2d, 0xd0, 0x1e, 0x17, 0x23, 0xf1, 0x18, 0x1e, 0x31, 0xce, 0xb5, 0xfb,
    0x2c, 0xe7, 0x5c, 0xed, 0x30, 0x48, 0x1e, 0x0f, 0x2f, 0x5e, 0xe6, 0x51,
    0x91, 0xe0, 0x51, 0x43, 0x8e, 0x73, 0x4f, 0x8c, 0xe7, 0x08, 0xec, 0x03,
    0x1f, 0x42, 0x5c, 0xe8, 0x15, 0x2c, 0xeb, 0x3f, 0x21, 0xce, 0x10, 0x75,
    0x48, 0xed, 0x25, 0x61, 0xe0, 0xf2, 0xf4, 0xee, 0x60, 0x91, 0xe8, 0x55,
    0x03, 0x18, 0xe8, 0x22, 0xf9, 0xce, 0x64, 0x0d, 0xec, 0x52, 0x1f, 0xf1,
    0x5c, 0xed, 0x15, 0x4c, 0xed, 0xf3, 0xae, 0xc0, 0x09, 0x3f, 0x25, 0x48,
    0xed, 0x06, 0x1e, 0x0f, 0x2f, 0x3e, 0xe6, 0x01, 0xe3, 0x65, 0x13, 0x1f,
    0x52, 0x7e, 0x82, 0x91, 0xe8, 0x54, 0x09, 0xce, 0x85, 0x11, 0xf2, 0x25,
    0x61, 0xea, 0x92, 0x54, 0x8e, 0xcd, 0xec, 0x13, 0xf8, 0x25, 0x47, 0xc0,
    0xce, 0xd3, 0xf4, 0xbe, 0xc0, 0x11, 0x51, 0x13, 0x3f, 0x22, 0x0c, 0x1e,
    0x0c, 0x15, 0x21, 0x3d, 0x1e, 0x26, 0x52, 0x1f, 0x33, 0x04, 0x81, 0xe2,
    0x92, 0x01, 0x43, 0x78, 0x51, 0x5f, 0x02, 0x20, 0x3c, 0xec, 0x23, 0xf1,
    0x3d, 0x1e, 0x27, 0x23, 0x6f, 0x23, 0x15, 0x14, 0x35, 0x13, 0xd1, 0xe3,
    0x44, 0x13, 0x2f, 0x32, 0x54, 0x81, 0xe2, 0xd3, 0x11, 0x01, 0x13, 0x81,
    0xf0, 0xa1, 0xe1, 0xc1, 0x5f, 0x12, 0x72, 0xe0, 0xb9, 0x1f, 0x33, 0x3f,
    0xd1, 0xe0, 0xc1, 0x41, 0x13, 0x4f, 0x12, 0x51, 0x40, 0xc1, 0xe1, 0xca,
    0x81, 0xf1, 0x9d, 0x2e, 0x26, 0x13, 0x6f, 0x23, 0x06, 0x2e, 0x2c, 0xd9,
    0x4f, 0x1a, 0xc2, 0xe0, 0xc1, 0x42, 0x51, 0x36, 0xf2, 0x31, 0x51, 0x40,
    0x73, 0xe0, 0xc3, 0xc0, 0xe2, 0xe3, 0x86, 0x02, 0xa1, 0xf3, 0x20, 0x46,
    0x1c, 0x0e, 0x5e, 0x2c, 0x85, 0x14, 0x25, 0x13, 0xa1, 0xf2, 0x31, 0x03,
    0x46, 0x67, 0x8e, 0xc7, 0x61, 0x42, 0x51, 0x2e, 0x1f, 0x23, 0x20, 0x15,
    0x34, 0x8e, 0x62, 0x44, 0x50, 0x12, 0x3f, 0x1f, 0x23, 0x21, 0x10, 0x75,
    0x6e, 0x80, 0x12, 0x3b, 0x1f, 0x09, 0x6c, 0x0d, 0x1f, 0x63, 0xde, 0x60,
    0x23, 0x83, 0xf4, 0x30, 0x6e, 0xd1, 0xf1, 0x2d, 0x5e, 0x1c, 0x94, 0xf1,
    0x20, 0x55, 0x70, 0x3f, 0xde, 0x60, 0x39, 0x3f, 0x73, 0x06, 0xed, 0xf3,
    0x05, 0x51, 0x02, 0x5f, 0x13, 0x25, 0x17, 0x23, 0xfd, 0xe6, 0x03, 0x93,
    0xf4, 0x30, 0x57, 0xab, 0x3f, 0x42, 0x05, 0x13, 0xa3, 0xf0, 0x21, 0x10,
    0x3c, 0x3f, 0x13, 0x3e, 0x1f,
};  // 581 bytes

LINKERSECTION(".extra")
/*static*/ const unsigned char CWndModuleSelector::iconAbout[] = {
    0x47, 0x42, 0x24, 0x24, 0x48, 0x20, 0x30, 0x20, 0x20, 0x20, 0x10, 0x41,
    0x90, 0x20, 0x70, 0x20, 0x71, 0xa0, 0x4a, 0x60, 0x3b, 0xe0, 0x10, 0xe0,
    0x11, 0xa0, 0x0a, 0x60, 0x16, 0x00, 0x0b, 0xe0, 0x07, 0xc0, 0x00, 0x00,
    0xb1, 0xf0, 0xa1, 0xd0, 0xc3, 0xc1, 0xda, 0x83, 0xf1, 0x9d, 0xb1, 0xe1,
    0xd9, 0x82, 0xf0, 0xdf, 0x1e, 0x0d, 0x92, 0xf1, 0xac, 0x92, 0xe1, 0xc9,
    0xa1, 0xf0, 0xad, 0x2e, 0x0b, 0xc1, 0xf0, 0xbf, 0x2e, 0x0a, 0x6f, 0x09,
    0xd1, 0xe3, 0xda, 0xbc, 0x7e, 0x0a, 0x81, 0xf1, 0x3c, 0x7e, 0x0a, 0x2f,
    0x0d, 0xc1, 0xe0, 0xc4, 0xf0, 0xdd, 0x1e, 0x0b, 0x2f, 0x0a, 0x81, 0xe1,
    0xd3, 0x5f, 0x01, 0x91, 0xe1, 0xc9, 0x1f, 0x0c, 0xe1, 0xe0, 0xf1, 0xf0,
    0xd8, 0x2e, 0x0c, 0xb1, 0xe1, 0x82, 0x3f, 0x01, 0xe3, 0xe2, 0xcf, 0xad,
    0x1e, 0x1d, 0xb1, 0xa0, 0xcc, 0x1e, 0x17, 0x21, 0xf1, 0x18, 0xc1, 0xe0,
    0xa3, 0xf1, 0xdc, 0xb1, 0xe1, 0xdd, 0xa1, 0xe3, 0xcc, 0xed, 0x2f, 0x0a,
    0xc1, 0xe1, 0xc0, 0x13, 0x10, 0xcc, 0x1e, 0x0d, 0x2f, 0x09, 0x1e, 0x1c,
    0xc9, 0x1e, 0x1c, 0xcc, 0x1e, 0x0c, 0x3f, 0x0e, 0xc1, 0xe1, 0xc5, 0x12,
    0x15, 0xcd, 0x1e, 0x0b, 0x2f, 0x0a, 0xc1, 0xe1, 0xcc, 0xb1, 0xe0, 0xc3,
    0xf0, 0xcd, 0x1e, 0x1c, 0x51, 0x21, 0x5c, 0xe1, 0xe0, 0x92, 0xf0, 0xbb,
    0x1e, 0x1d, 0xbb, 0x1e, 0x0f, 0x2f, 0x0d, 0xe1, 0xe1, 0x85, 0x12, 0x15,
    0x6f, 0x1e, 0x0f, 0x2f, 0x0d, 0xa1, 0xe2, 0x93, 0xc9, 0x1e, 0x09, 0x2f,
    0x0b, 0x1e, 0x1c, 0xca, 0x1e, 0x2c, 0x40, 0x13, 0x20, 0x47, 0xb1, 0xe3,
    0xdc, 0xec, 0x3f, 0x0c, 0x81, 0xe0, 0xb1, 0xf1, 0x2c, 0x7e, 0x0d, 0x3f,
    0x0b, 0x1d, 0x0e, 0xb1, 0xe2, 0x45, 0x11, 0xf3, 0x30, 0x48, 0xc1, 0xe1,
    0xcb, 0x3f, 0x0d, 0x7e, 0x17, 0x32, 0xf1, 0x28, 0x7e, 0x3d, 0xbd, 0xcd,
    0x1e, 0x3c, 0x45, 0x13, 0xf1, 0x20, 0x14, 0x0e, 0xf2, 0xe2, 0xc0, 0x34,
    0xf2, 0x20, 0xcd, 0x2e, 0x0c, 0x14, 0x15, 0x15, 0xf2, 0x31, 0x51, 0x40,
    0xcb, 0x2e, 0x2c, 0x52, 0x81, 0xf2, 0x15, 0x89, 0x2e, 0x08, 0x14, 0x25,
    0x13, 0x81, 0xf2, 0x31, 0x51, 0x41, 0x6c, 0xd1, 0xe3, 0x86, 0x51, 0xc1,
    0xf5, 0x20, 0x46, 0x8c, 0x7e, 0x2c, 0x86, 0x24, 0x15, 0x1e, 0x1f, 0x22,
    0x05, 0xd1, 0x42, 0x51, 0x29, 0x2f, 0x32, 0x10, 0x57, 0x40, 0x51, 0x01,
    0x23, 0xc2, 0xf1, 0x22, 0x71, 0x12, 0x3b, 0x1f,
};  // 464 bytes

LINKERSECTION(".extra")
/*static*/ const unsigned char CWndModuleSelector::iconUser[] = {
    0x47, 0x42, 0x25, 0x27, 0xa0, 0x00, 0x68, 0x00, 0x00, 0x00, 0x38, 0x00,
    0x18, 0x00, 0x79, 0x40, 0x72, 0x60, 0x4c, 0x40, 0x01, 0x60, 0x0a, 0xe0,
    0x15, 0xa0, 0x16, 0x60, 0x0c, 0x80, 0x07, 0x80, 0x07, 0x00, 0x07, 0xe0,
    0xa1, 0x21, 0x88, 0x94, 0x20, 0x81, 0xc0, 0x97, 0x21, 0x8e, 0x1f, 0x0a,
    0xa2, 0x20, 0xc3, 0xf1, 0xd8, 0x52, 0x0a, 0x3f, 0x08, 0x82, 0x21, 0x4b,
    0x4f, 0x0a, 0x32, 0x14, 0xb5, 0xf0, 0x8f, 0x12, 0x09, 0x6f, 0x19, 0x41,
    0x20, 0x96, 0xf1, 0x34, 0xd1, 0x21, 0x46, 0x6f, 0x0e, 0x12, 0x13, 0xa6,
    0xf0, 0xcf, 0x12, 0x0c, 0x6f, 0x3d, 0x34, 0x87, 0xf1, 0x63, 0xc1, 0x22,
    0x43, 0x67, 0xf2, 0xc4, 0x57, 0xf0, 0xcf, 0x12, 0x09, 0x7f, 0x25, 0x3a,
    0x7f, 0x25, 0x34, 0xc1, 0x22, 0x30, 0xb6, 0xf2, 0xb3, 0x57, 0xf0, 0x8f,
    0x12, 0x14, 0xe6, 0xf2, 0x03, 0xa6, 0xf2, 0x70, 0x3d, 0x12, 0x34, 0x10,
    0xd5, 0xf3, 0xc3, 0x0a, 0x5f, 0x09, 0x22, 0x4c, 0xed, 0xa8, 0x12, 0x0b,
    0x3f, 0x4e, 0x82, 0x3b, 0x4f, 0x36, 0x03, 0x95, 0xf3, 0x60, 0x14, 0xa1,
    0x27, 0xcc, 0x92, 0x31, 0x07, 0x3f, 0x0a, 0x14, 0x21, 0x07, 0x2f, 0x1a,
    0x31, 0x40, 0xb5, 0xf1, 0x9c, 0x6f, 0x3c, 0x43, 0x11, 0x60, 0x51, 0x00,
    0x31, 0x44, 0x37, 0xb7, 0x61, 0x02, 0x19, 0xd2, 0xf1, 0xe8, 0x22, 0x14,
    0xc5, 0xf1, 0x81, 0x30, 0x11, 0x41, 0x21, 0x41, 0x20, 0x41, 0x34, 0x3d,
    0x6f, 0x1c, 0xc7, 0xf3, 0x64, 0x24, 0x23, 0x04, 0x32, 0x03, 0x21, 0x23,
    0x4e, 0x5f, 0x4e, 0x32, 0x41, 0x7f, 0x0c, 0x34, 0x02, 0x52, 0x04, 0x22,
    0x13, 0x77, 0xf1, 0x94, 0x7f, 0x17, 0x34, 0x22, 0x89, 0x86, 0x20, 0xe6,
    0xf5, 0xd1, 0x43, 0x0b, 0x7f, 0x08, 0x32, 0x19, 0xe2, 0xf1, 0xe9, 0x12,
    0x24, 0x17, 0x6f, 0x4c, 0x42, 0x3b, 0x5f, 0x46, 0x14, 0x2b, 0x6f, 0x0a,
    0x22, 0x08, 0x7f, 0x67, 0x03, 0x41, 0x0d, 0x6f, 0x08, 0x12, 0x0a, 0x81,
    0xf4, 0xe8, 0x10, 0xa3, 0xf2, 0xe5, 0x42, 0x25, 0x31, 0x7e, 0xd7, 0x10,
    0x13, 0xea, 0x1f, 0x0a, 0x12, 0x0f, 0x5f, 0x57, 0x01, 0x42, 0x31, 0x00,
    0x74, 0xf2, 0xa4, 0xcc, 0x1f, 0x1b, 0x13, 0x02, 0x13, 0x44, 0x21, 0x43,
    0x31, 0x0a, 0xe1, 0xf2, 0x93, 0xb1, 0xf1, 0xd7, 0x10, 0x11, 0x42, 0x21,
    0x41, 0x20, 0x46, 0x51, 0x3e, 0xf1, 0xf0, 0x92, 0x40, 0x2a, 0x12, 0x18,
    0xd8, 0x2f, 0x2c, 0x31, 0x10, 0x21, 0x14, 0x62, 0x04, 0x43, 0x0d, 0x92,
    0xf0, 0xcc, 0x12, 0x08, 0xb2, 0xf1, 0x74, 0xf1, 0x21, 0x3b, 0xb2, 0xf0,
    0xcb, 0x12, 0x0e, 0xc2, 0xf1, 0x34, 0xd1, 0x21, 0x46, 0xd2, 0xf0, 0x29,
    0x12, 0x08, 0xd2, 0xf1, 0x53, 0xd1, 0x22, 0x30, 0x7b, 0x2f, 0x0b, 0xb1,
    0x21, 0x3a, 0x4f, 0x3d, 0xa7, 0x61, 0x02, 0x57, 0xd3, 0xf0, 0x71, 0x00,
    0x3d, 0x12, 0x14, 0x12, 0x00, 0x61, 0x60, 0x08, 0x10, 0x05, 0x16, 0x25,
    0x14, 0xc1, 0x21, 0x31, 0x50, 0x11, 0x13, 0x31, 0x11, 0x30, 0x11, 0x48,
    0x22, 0x04, 0x43, 0x04, 0x32, 0x14, 0x45, 0x30, 0x4a, 0x22, 0x04, 0xb1,
    0x20, 0x4b, 0x12,
};  // 435 bytes
