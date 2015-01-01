/*
 * Copyright (C) 2014 Antti Aura
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the author nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "WalkTimer.h"
#include <QTimer>
#include <QDateTime>


WalkTimer::WalkTimer(QObject* parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    enable();
}

WalkTimer::~WalkTimer(void)
{
    delete m_timer, m_timer = 0;
}

void WalkTimer::disable(void)
{
    m_timer->stop();
}

void WalkTimer::enable(void)
{
    update();
    m_timer->start(1000);
}

QString WalkTimer::getWaitingDuration(void) const
{
    return m_waitingDurationStr;
}

QString WalkTimer::getWalkDuration(void) const
{
    return m_walkDurationStr;
}

void WalkTimer::startWalk(void)
{
    QDateTime t;
    m_walkStart = t.currentDateTime().toTime_t();
}

void WalkTimer::finishWalk(void)
{
    QDateTime t;
    m_walkEnd = t.currentDateTime().toTime_t();
    m_waitingStart = m_walkEnd;
    m_walkDuration = m_walkEnd-m_walkStart;
}

int WalkTimer::getWalkLenght(void) const
{
    return m_walkDuration;
}

int WalkTimer::getWalkStart(void) const
{
    return m_walkStart;
}

int WalkTimer::getWalkEnd(void) const
{
    return m_walkEnd;
}

void WalkTimer::update(void)
{
    QDateTime dt;
    int t = dt.currentDateTime().toTime_t();

    m_waitingDurationStr = durarationStr(m_waitingStart, t);
    m_walkDurationStr = durarationStr(m_walkStart, t);
    emit waitingDurationChanged(m_waitingDurationStr);
    emit walkDurationChanged(m_walkDurationStr);
}

QString WalkTimer::durarationStr(const int startTime, const int timeNow)
{
    int diff = timeNow-startTime;

    if (diff < 0 || diff > 7*86400) {
        return QString("--:--:--");
    }

    int hours = diff/3600;
    diff = diff%3600;

    int minutes = diff/60;
    diff = diff%60;

    int seconds = diff;

    QTime s;
    s.setHMS(hours, minutes, seconds);
    return s.toString();
}