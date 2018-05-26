/*
 * Copyright (C) 2017-2018 Tauri JayD <https://www.>
 * Copyright (c) 2014 Steven Kirk <The Avalonia Project>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __XUI_UI_RADIOBUTTON_H__
#define __XUI_UI_RADIOBUTTON_H__

#include "UI/Detail/ToggleButton.h"

namespace XUI::UI
{
    class TC_XUI_API RadioButton : public ToggleButton
    {
    public:
        static Core::Dependency::RefDependencyProperty GroupNameProperty;

    private:
        bool _CheckedStateOnChanging = false;

    protected:

        void Toggle() override;

    public:

        RadioButton();

        virtual ~RadioButton();

        static void StaticClassInitializer();

        PROPERTY_(std::string, GroupName);
        std::string GetGroupName() const;
        void SetGroupName(std::string const& value);

        void IsCheckedChanged(ToggleButtonState state);
    };
}

#endif // !__XUI_UI_RADIOBUTTON_H__