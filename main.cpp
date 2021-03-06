#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDesktopWidget>
#include <iostream>

/*!
\mainpage Scan Studio

Scan Studio allows users to quickly and easily scan 3D objects into the
industry-standard STL format.

\section attrib Attribution

\subsection Oxygen Oxygen icon set

<a href="http://www.oxygen-icons.org/">Oxygen</a> icon set used under the
terms of the <a href="http://creativecommons.org/licenses/by-sa/3.0/">
CC BY-SA 3.0</a> license.

\subsection qt Qt

<a href="http://qt-project.org/">Qt</a> application framework used under the
terms of the <a href="http://www.gnu.org/licenses/lgpl-3.0-standalone.html">
GNU Lesser General Public License</a>.

\subsection opencv OpenCV

<a href="http://opencv.org/">OpenCV</a> computer vision library used under the
terms of the <a href="http://opensource.org/licenses/BSD-2-Clause">BSD 2-Clause
License</a>.

\section license License

Copyright &copy; 2013, Ryan Muller and Chris Thomas.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
<ul>
<li> Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.</li>
<li> Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.</li>
</ul>

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include <QString>
#include <opencv2/calib3d/calib3d.hpp>
#include "geom/triangulator.h"
#include "hardware/projector/sinusoidpattern.h"

/// Entry point for ScanStudio.
int main(int argc, char** argv)
{
//    SinusoidPattern *patt = new SinusoidPattern(64,0);
//    QImage pattern = patt->generatePattern(800,600);
//    pattern.save(QString("/home/ryan/sinusoid.png"));
//    return 0;
    QApplication a(argc, argv);
    MainWindow w;
    QRect screenres = QApplication::desktop()->screenGeometry(0);
    w.move(QPoint(screenres.x(),screenres.y()));
    w.resize(screenres.width(),screenres.height());
    w.showMaximized();
    return a.exec();
}
