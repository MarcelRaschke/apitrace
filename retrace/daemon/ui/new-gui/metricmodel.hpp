/**************************************************************************
 *
 * Copyright 2017 Intel Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Laura Ekstrand <laura@jlekstrand.net>
 **************************************************************************/


#ifndef _METRICMODEL_HPP_
#define _METRICMODEL_HPP_

#include <string>
#include <vector>

#include <QHash>
#include <QObject>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QVector>

#include "glframe_retrace_interface.hpp"

namespace glretrace {

class MetricModel : public QObject {
  Q_OBJECT

 public:
  explicit MetricModel(const std::vector<MetricId> &idsVector,
                       const std::vector<std::string> &namesVector,
                       const std::vector<std::string> &descVector,
                       QObject *parent = 0);
  ~MetricModel();

  MetricId getId(QString name);
  // QVector, QList, and QStringList are implicitly shared, so this is fast.
  QString getName(MetricId id);
  QVector<QString> getNames();
  QStringList getNamesList();
  QStandardItemModel* getTable() { return tableModel; }
  void setRender(int idx) { index = idx; }

 signals:
  void tableReady(QStandardItemModel* tbl);

 public slots:
  void updateTableData(QString name, float data);

 private:
  static QString hashId(MetricId id);
  int getNumericalIndex(QString name);

 protected:
  // This is redundant, because QVector has a find() function.
  // Fix later.
  QHash<QString, int> indicesByName;
  QHash<QString, int> indicesById;
  QVector<QString> names;
  QVector<MetricId> ids;
  QVector<QString> descriptions;
  QStandardItemModel *tableModel;
  int index;
};

}  // namespace glretrace

#endif  // _METRICMODEL_HPP_