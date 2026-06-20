/** <pre>
 *  Copyright 2004 The Boeing Company
 *  Author: Lance Durham
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2.1 of the License, or (at
 *  your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * </pre>
 */

#ifndef _CONFIGTEMPL_H_
#define _CONFIGTEMPL_H_

#include "ConfigTemplComponent.h"
#include "ConfigTemplArtPart.h"
#include "ConfigTemplAttachPoint.h"
#include "ConfigTemplCollisionSeg.h"
#include "ConfigTemplCollisionVol.h"
#include "ConfigTemplEntity.h"
#include "ConfigTemplView.h"
#include "ConfigTemplViewGroup.h"
#include "ConfigTemplDatabase.h"
#include "ConfigTemplGlobalEnv.h"
#include "ConfigTemplWeather.h"

void SetFilenameForConfigErrorMessages(LPCTSTR filename);

#endif // _CONFIGTEMPL_H_