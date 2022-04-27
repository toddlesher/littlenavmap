/*****************************************************************************
* Copyright 2015-2022 Alexander Barthel alex@littlenavmap.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "profile/profileoptions.h"

#include "gui/treedialog.h"
#include "common/constants.h"
#include "settings/settings.h"

#include <QDebug>

ProfileOptions::ProfileOptions(QWidget *parentWidgetParam)
  : parentWidget(parentWidgetParam)
{

}

bool ProfileOptions::showOptions()
{
  qDebug() << Q_FUNC_INFO;

  atools::gui::TreeDialog treeDialog(parentWidget, QApplication::applicationName() + tr(" - Elevation Profile"),
                                     tr("Select options for Elevation Profile."),
                                     lnm::PROFILE_DISPLAY_OPTIONS_DIALOG, "PROFILE.html#profile-options",
                                     true /* showExpandCollapse */);
  treeDialog.setHelpOnlineUrl(lnm::helpOnlineUrl);
  treeDialog.setHelpLanguageOnline(lnm::helpLanguageOnline());
  treeDialog.setHeader({tr("Option"), tr("Description")});

  // Profile =====================================================
  /* *INDENT-OFF* */
  QTreeWidgetItem* rootItem = treeDialog.getRootItem();
  treeDialog.addItem2(rootItem, optsp::PROFILE_TOOLTIP, tr("Show Tooltip"), tr("Show a tooltip with elevation and more information when hovering the mouse over the profile."), QString());
  treeDialog.addItem2(rootItem, optsp::PROFILE_ALT_LABELS, tr("Show Altitude Labels"), tr("Show or hide the labels at the left side of the elevation profile."), QString());

  QTreeWidgetItem *topItem = treeDialog.addTopItem1(tr("Top Label"));
  treeDialog.addItem2(topItem, optsp::PROFILE_TOP_DISTANCE, tr("Distance"), tr("Distance of flight plan leg."), QString());
  treeDialog.addItem2(topItem, optsp::PROFILE_TOP_MAG_COURSE, tr("Magnetic Course"), tr( "Show magnetic great circle start course at start flight plan leg."), QString());
  treeDialog.addItem2(topItem, optsp::PROFILE_TOP_TRUE_COURSE, tr("True Course"), tr( "Show true great circle start course at flight plan leg."), QString());
  treeDialog.addItem2(topItem, optsp::PROFILE_TOP_RELATED, tr("Related Navaid"), tr( "Related navaid for a procedure fix including bearing and distance."), QString());

  QTreeWidgetItem *lineItem = treeDialog.addTopItem1(tr("Flight Plan Line"));
  treeDialog.addItem2(lineItem, optsp::PROFILE_FP_DIST, tr("Distance"), tr("Distance of flight plan leg."), QString());
  treeDialog.addItem2(lineItem, optsp::PROFILE_FP_MAG_COURSE, tr("Magnetic Course"), tr( "Show magnetic great circle start course."), QString());
  treeDialog.addItem2(lineItem, optsp::PROFILE_FP_TRUE_COURSE, tr("True Course"), tr("Show true great circle start course."), QString());
  treeDialog.addItem2(lineItem, optsp::PROFILE_FP_VERTICAL_ANGLE, tr("Descent Flight Path Angle"), tr("Vertical descent path angle only in the descent phase."), QString());
  treeDialog.addItem2(lineItem, optsp::PROFILE_FP_ALT_RESTRICTION, tr("Altitude Restriction"), tr( "Display procedure altitude restrictions at the navaid label."), QString());
  treeDialog.addItem2(lineItem, optsp::PROFILE_FP_SPEED_RESTRICTION, tr("Speed Restriction"), tr("Show procedure speed restrictions at the navaid label."), QString());
  /* *INDENT-ON* */

  treeDialog.restoreState(false /* restoreCheckState */, true /* restoreExpandState */);

  // Restoring in dialog is disabled - restore options manually
  for(optsp::DisplayOptionProfile opt : optsp::ALL_OPTIONS)
    treeDialog.setItemChecked(opt, displayOptions.testFlag(opt));

  if(treeDialog.exec() == QDialog::Accepted)
  {
    treeDialog.saveState(false /* saveCheckState */, true /* saveExpandState */);

    // Extract options from check items
    for(optsp::DisplayOptionProfile opt : optsp::ALL_OPTIONS)
      displayOptions.setFlag(opt, treeDialog.isItemChecked(opt));

    return true;
  }
  return false;
}

void ProfileOptions::saveState()
{
  atools::settings::Settings::instance().setValue(lnm::PROFILE_DISPLAY_OPTIONS, static_cast<int>(displayOptions));
}

void ProfileOptions::restoreState()
{
  const atools::settings::Settings& settings = atools::settings::Settings::instance();

  if(settings.contains(lnm::PROFILE_DISPLAY_OPTIONS))
    displayOptions = optsp::DisplayOptionsProfile(settings.valueInt(lnm::PROFILE_DISPLAY_OPTIONS));
  // else use default
}
