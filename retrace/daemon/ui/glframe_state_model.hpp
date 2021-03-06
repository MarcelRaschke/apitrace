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
 *   Mark Janes <mark.a.janes@intel.com>
 **************************************************************************/

#ifndef _GLFRAME_STATE_MODEL_HPP_
#define _GLFRAME_STATE_MODEL_HPP_

#include <QObject>
#include <QList>
#include <QQmlListProperty>
#include <QString>
#include <QVariant>

#include <mutex>
#include <string>
#include <map>
#include <vector>

#include "glframe_retrace_interface.hpp"
#include "glframe_traits.hpp"

namespace glretrace {

class QStateValue : public QObject, NoCopy, NoAssign, NoMove {
  Q_OBJECT

  Q_PROPERTY(QString path READ path CONSTANT)
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QStringList value READ value NOTIFY valueChanged)
  Q_PROPERTY(QVariant indent READ indent CONSTANT)
  Q_PROPERTY(QVariant visible
             READ visible
             WRITE setVisible
             NOTIFY visibleChanged)
  Q_PROPERTY(QStringList choices READ choices CONSTANT)
  Q_PROPERTY(QStringList indices READ indices CONSTANT)

 public:
  explicit QStateValue(QObject *parent = 0);
  QStateValue(QObject *parent,
              const std::string &_path,
              const std::string &_name,
              const std::vector<std::string> &_choices,
              const std::vector<std::string> &_indices);
  void insert(const std::vector<std::string> &value);

  QString path() const { return m_path; }
  QString name() const { return m_name; }
  QStringList value() const { return m_value; }
  QVariant indent() const { return m_indent; }
  QVariant visible() const { return m_visible; }
  void setVisible(QVariant v) { m_visible = v; emit visibleChanged(); }
  QStringList choices() const { return m_choices; }
  QStringList indices() const { return m_indices; }
  void clear() { m_value.clear(); emit valueChanged(); }

 signals:
  void visibleChanged();
  void valueChanged();

 private:
  QString value_to_choice(const std::string &_value);

  QString m_path, m_name;
  QStringList m_value;
  QVariant m_indent, m_visible;
  QStringList m_choices;
  QStringList m_indices;
};

class QStateModel : public QObject,
                    NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(QQmlListProperty<glretrace::QStateValue> state READ state
             NOTIFY stateChanged)
 public:
  QStateModel();
  explicit QStateModel(IFrameRetrace *retrace);
  ~QStateModel();
  QQmlListProperty<QStateValue> state();
  void onState(SelectionId selectionCount,
               ExperimentId experimentCount,
               RenderId renderId,
               StateKey item,
               const std::vector<std::string> &value);
  void clear();
  Q_INVOKABLE void setState(const QString &path,
                            const QString &name,
                            int offset,
                            const QString &value);
  Q_INVOKABLE void collapse(const QString &path);
  Q_INVOKABLE void expand(const QString &path);
  Q_INVOKABLE void search(const QString &_search);

 signals:
  void stateExperiment();
  void stateChanged();

 private:
  void refresh();
  void set_visible();

  IFrameRetrace *m_retrace;
  SelectionId m_sel_count;
  ExperimentId m_experiment_count;
  std::map<StateKey, QStateValue*> m_state_by_name;
  std::map<std::string, bool> m_filter_paths;
  std::map<std::string, bool> m_known_paths;
  QList<QStateValue*> m_states;
  std::vector<RenderId> m_renders;
  QString m_search;
  mutable std::mutex m_protect;
};

}  // namespace glretrace

#endif  // _GLFRAME_STATE_MODEL_HPP_
