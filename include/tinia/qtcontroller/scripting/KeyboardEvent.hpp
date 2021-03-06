/* Copyright STIFTELSEN SINTEF 2012
 * 
 * This file is part of the Tinia Framework.
 * 
 * The Tinia Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * The Tinia Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with the Tinia Framework.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <QObject>
#include <QKeyEvent>

namespace tinia {
namespace qtcontroller {
namespace scripting {

class KeyboardEvent : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardEvent(const QKeyEvent& event, QObject *parent = 0);
    Q_PROPERTY(int key READ key)
signals:
    
public slots:
    int key();
private:
    const QKeyEvent& m_event;
};

} // namespace scripting
} // namespace qtcontroller
} // namespace tinia

