/*
 * Common Public Attribution License Version 1.0. 
 * 
 * The contents of this file are subject to the Common Public Attribution 
 * License Version 1.0 (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License 
 * at http://www.xTuple.com/CPAL.  The License is based on the Mozilla 
 * Public License Version 1.1 but Sections 14 and 15 have been added to 
 * cover use of software over a computer network and provide for limited 
 * attribution for the Original Developer. In addition, Exhibit A has 
 * been modified to be consistent with Exhibit B.
 * 
 * Software distributed under the License is distributed on an "AS IS" 
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See 
 * the License for the specific language governing rights and limitations 
 * under the License. 
 * 
 * The Original Code is xTuple ERP: PostBooks Edition 
 * 
 * The Original Developer is not the Initial Developer and is __________. 
 * If left blank, the Original Developer is the Initial Developer. 
 * The Initial Developer of the Original Code is OpenMFG, LLC, 
 * d/b/a xTuple. All portions of the code written by xTuple are Copyright 
 * (c) 1999-2008 OpenMFG, LLC, d/b/a xTuple. All Rights Reserved. 
 * 
 * Contributor(s): ______________________.
 * 
 * Alternatively, the contents of this file may be used under the terms 
 * of the xTuple End-User License Agreeement (the xTuple License), in which 
 * case the provisions of the xTuple License are applicable instead of 
 * those above.  If you wish to allow use of your version of this file only 
 * under the terms of the xTuple License and not to allow others to use 
 * your version of this file under the CPAL, indicate your decision by 
 * deleting the provisions above and replace them with the notice and other 
 * provisions required by the xTuple License. If you do not delete the 
 * provisions above, a recipient may use your version of this file under 
 * either the CPAL or the xTuple License.
 * 
 * EXHIBIT B.  Attribution Information
 * 
 * Attribution Copyright Notice: 
 * Copyright (c) 1999-2008 by OpenMFG, LLC, d/b/a xTuple
 * 
 * Attribution Phrase: 
 * Powered by xTuple ERP: PostBooks Edition
 * 
 * Attribution URL: www.xtuple.org 
 * (to be included in the "Community" menu of the application if possible)
 * 
 * Graphic Image as provided in the Covered Code, if any. 
 * (online at www.xtuple.com/poweredby)
 * 
 * Display of Attribution Information is required in Larger Works which 
 * are defined in the CPAL as a work which combines Covered Code or 
 * portions thereof with code not governed by the terms of the CPAL.
 */

#include <xsqlquery.h>
#include <parameter.h>

#include "vendorgroup.h"

VendorGroup::VendorGroup(QWidget *pParent, const char *pName) : QWidget(pParent, pName)
{
  setupUi(this);

  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

  _select->setCurrentIndex(All);
  _vend->setId(-1);
  _vendorTypes->setId(-1);
  _vendorType->setText("");

  connect(_select,SIGNAL(currentIndexChanged(int)), this, SIGNAL(updated()));
  connect(_select,SIGNAL(currentIndexChanged(int)), this, SIGNAL(newState(int)));
  connect(_vend,                SIGNAL(newId(int)), this, SIGNAL(updated()));
  connect(_vend,                SIGNAL(newId(int)), this, SIGNAL(newVendId(int)));
  connect(_vendorTypes,         SIGNAL(newID(int)), this, SIGNAL(updated()));
  connect(_vendorTypes,         SIGNAL(newID(int)), this, SIGNAL(newVendTypeId(int)));
  connect(_vendorType,   SIGNAL(editingFinished()), this, SIGNAL(updated()));
  connect(_vendorType,   SIGNAL(editingFinished()), this, SLOT(sTypePatternFinished()));
  connect(_vendorType,SIGNAL(textChanged(QString)), this, SIGNAL(newTypePattern(QString)));

  setFocusProxy(_select);
}

VendorGroup::~VendorGroup()
{
}

void VendorGroup::languageChange()
{
  retranslateUi(this);
}

void VendorGroup::appendValue(ParameterList &pParams)
{
  switch (_select->currentIndex())
  {
    case Selected:
      pParams.append("vend_id", _vend->id());
      break;
    case SelectedType:
      pParams.append("vendtype_id", _vendorTypes->id());
      break;
    case TypePattern:
      pParams.append("vendtype_pattern", _vendorType->text());
      break;
    case All:
    default:
      ; // nothing
  }
}

void VendorGroup::bindValue(XSqlQuery &pQuery)
{
  switch (_select->currentIndex())
  {
    case Selected:
      pQuery.bindValue(":vend_id", _vend->id());
      break;
    case SelectedType:
      pQuery.bindValue(":vendtype_id", _vendorTypes->id());
      break;
    case TypePattern:
      pQuery.bindValue(":vendtype_pattern", _vendorType->text());
      break;
    case All:
    default:
      ; // nothing
  }
}

bool VendorGroup::isValid()
{
  switch (_select->currentIndex())
  {
    case Selected:
      return _vend->isValid();
      break;
    case SelectedType:
      return _vendorTypes->isValid();
      break;
    case TypePattern:
      return ! _vendorType->text().trimmed().isEmpty();
      break;
    case All:
      return true;
      break;
  }

  return false;
}

void VendorGroup::setVendId(int p)
{
  _vend->setId(p);
  setState(Selected);
}

void VendorGroup::setVendTypeId(int p)
{
  _vendorTypes->setId(p);
  setState(SelectedType);
}

void VendorGroup::setTypePattern(const QString &p)
{
  _vendorType->setText(p);
  sTypePatternFinished();
  setState(TypePattern);
}

void VendorGroup::setState(enum VendorGroupState p)
{
  _select->setCurrentIndex(p);
}

void VendorGroup::sTypePatternFinished()
{
  emit newTypePattern(_vendorType->text());
  emit updated();
}

/*
   keep the contents of another vendor group (p) synchronized with this in a
   manner which allows any changes on this to propagate to p and any objects
   listening for signals from p will respond to changes to this.
   hide p because it's superfluous.
*/
void VendorGroup::synchronize(VendorGroup *p)
{
  connect(this, SIGNAL(newTypePattern(QString)), p, SLOT(setTypePattern(QString)));
  connect(this, SIGNAL(newState(int)),           p, SLOT(setState(int)));
  connect(this, SIGNAL(newVendId(int)),          p, SLOT(setVendId(int)));
  connect(this, SIGNAL(newVendTypeId(int)),      p, SLOT(setVendTypeId(int)));
  p->hide();
}
