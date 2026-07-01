// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

class QWidget;

namespace McStudioGui
{

class McStudioRibbonBackend: public QObject
{
public:
    explicit McStudioRibbonBackend(QObject* parent = nullptr)
        : QObject(parent)
    {}

    ~McStudioRibbonBackend() override = default;

    virtual QWidget* widget() = 0;
};

}  // namespace McStudioGui
