#ifndef QTOBSERVER_TABWIDGETCHILDREN_HPP
#define QTOBSERVER_TABWIDGETCHILDREN_HPP

#include <QWidget>
#include <QTableWidget>
#include "tinia/policylib/PolicyLib.hpp"
#include "tinia/policylib/StateSchemaListener.hpp"

namespace tinia {
namespace qtobserver {

class TabWidgetChildren : public QWidget, public policylib::StateSchemaListener
{
    Q_OBJECT
public:
    explicit TabWidgetChildren(std::string key,
                               std::shared_ptr<policylib::PolicyLib> policyLib,
                               QTabWidget *parent = 0);
   void stateSchemaElementAdded(policylib::StateSchemaElement *stateSchemaElement) {}
   void stateSchemaElementRemoved(policylib::StateSchemaElement *stateSchemaElement) {}
   void stateSchemaElementModified(policylib::StateSchemaElement *stateSchemaElement);

signals:

public slots:
private:
      std::shared_ptr<policylib::PolicyLib> m_policyLib;
      std::string m_key;

};

} // namespace qtobserver
} // of namespace tinia
#endif // QTOBSERVER_TABWIDGETCHILDREN_HPP
