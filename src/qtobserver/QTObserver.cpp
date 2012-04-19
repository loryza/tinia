#include "jobobserver/Job.hpp"
#include "jobobserver/OpenGLJob.hpp"
#include "qtobserver/QTObserver.hpp"
#include "qtobserver/GUIBuilder.hpp"
#include <QApplication>
#include <QLayout>
#include <QLabel>
#include <QGLWidget>
#include <QMainWindow>
#include <exception>

#include <QDomDocument>
#include <QString>

namespace qtobserver {

QTObserver::QTObserver()
    : m_main_window(NULL),
      m_root_context(NULL),
      m_job(NULL),
      m_builder(NULL),
      m_perf_mode( false ),
      m_renderlist_mode( false )
{
}

QTObserver::~QTObserver()
{
    if( m_main_window != NULL ){
        delete m_main_window;
    }

    if( m_job != NULL ) {
        delete m_job;
    }

    if( m_builder != NULL ) {
        delete m_builder;
    }
}




void QTObserver::setJob(jobobserver::Job *job)
{
    if( m_job != NULL ) {
        throw std::runtime_error( "Job already set" );
    }
    else {
       m_job = job;
    }
}

void QTObserver::notify()
{
}

void QTObserver::fail()
{
}

void QTObserver::finish()
{
}

int QTObserver::run(int argc, char **argv)
{
    if( m_job == NULL ) {
        throw std::runtime_error( "No job set" );
    }
    m_policyLib = m_job->getPolicylib();



    QApplication app(argc, argv);
    m_main_window = new QMainWindow();


    if( dynamic_cast<jobobserver::OpenGLJob*>( m_job ) ) {
        for( int i=1; i<argc; i++) {
            if( strcmp(argv[i], "--perf" ) == 0 ) {
                m_perf_mode = true;
            }
            else if( strcmp( argv[i], "--renderlist" ) == 0 ) {
                m_renderlist_mode = true;
            }
        }

        // Create an off-screen context that is subsequently shared with all
        // GL widgets that will be subsequently created. This avoids requiring
        // that the view is defined before doing GL init as well as allowing
        // GL resources to be shared between widgets.
        m_root_context = new QGLWidget(QGL::DepthBuffer| QGL::DoubleBuffer | QGL::AlphaChannel);
        m_root_context->makeCurrent();
    }
    if( !m_job->init() ) {
       throw new std::runtime_error("Job did not start up properly");
    }

    m_builder = new GUIBuilder( m_policyLib,
                                m_job,
                                this,
                                m_perf_mode,
                                m_root_context );

    m_main_window->setCentralWidget( m_builder->buildGUI( m_policyLib->getGUILayout(policylib::gui::DESKTOP),
                                                          NULL ) );

	if( dynamic_cast<jobobserver::OpenGLJob*>( m_job ) ) {
	if( !(dynamic_cast<jobobserver::OpenGLJob*>(m_job))->initGL()) {
		throw new std::runtime_error("Job did not initialize GL correctly");
	}
	}
    m_main_window->show();

/*
   QString style =
           "QGroupBox::title { color: black; } "
           "QGroupBox { margin: 0; border: 0; padding: 0; }";

   app.setStyleSheet( style );
*/
   /*
   policylib::gui::Element* tree = m_policyLib->getGUILayout( policylib::gui::DESKTOP );
   QDomDocument* dom = new QDomDocument;
   QDomElement gui_layout = dom->createElement( "layout" );
   dom->appendChild( gui_layout );
   addNode( dom, gui_layout, tree );
   QString foo = dom->toString();

   std::cerr << foo.toStdString() << "\n";
*/
   return app.exec();
}


static
QDomElement
addNode( QDomDocument* dom, QDomElement& parent, policylib::gui::Element* branch );

template<typename ChildType>
static
void
addChildren( QDomDocument* dom, QDomElement& parent, policylib::gui::Container1D<ChildType>* branch )
{
    for(size_t i=0; i<branch->children(); i++ ) {
        addNode( dom, parent, branch->child(i) );
    }
}

static
void
addChildren( QDomDocument* dom, QDomElement& parent, policylib::gui::Container2D<policylib::gui::Element>* branch )
{
    parent.setAttribute( "width", (uint)branch->width() );
    parent.setAttribute( "height", (uint)branch->height() );
    for(size_t j=0; j<branch->height(); j++ ) {
        for(size_t i=0; i<branch->width(); i++ ) {
            auto child = branch->child( j, i );
            if( child != NULL ) {
                QDomElement n = addNode( dom, parent, child );
                n.setAttribute( "row", (uint)j );
                n.setAttribute( "col", (uint)i );
            }
        }
    }
}

static
void
addElementAttributes( QDomDocument* dom, QDomElement& node, policylib::gui::Element* element )
{
    if( !element->visibilityKey().empty() ) {
        QDomElement n = dom->createElement( "Visibility" );
        QDomText t = dom->createTextNode( element->visibilityKey().c_str() );
        n.appendChild( t );
        if( element->visibilityInverted() ) {
            n.setAttribute( "inverted", 1 );
        }
        node.appendChild( n );
    }
    if( !element->enabledKey().empty() ) {
        QDomElement n = dom->createElement( "Enabled" );
        QDomText t = dom->createTextNode( element->enabledKey().c_str() );
        n.appendChild( t );
        if( element->enabledInverted() ) {
            n.setAttribute( "inverted", 1 );
        }
        node.appendChild( n );
    }
}

static
QDomElement
addNode( QDomDocument* dom, QDomElement& parent, policylib::gui::Element* branch )
{
    auto TabLayout = dynamic_cast<policylib::gui::TabLayout*>( branch );
    if( TabLayout ) {
        QDomElement node = dom->createElement( "TabLayout" );
        addElementAttributes( dom, node, branch );
        addChildren<policylib::gui::Tab>( dom, node, static_cast<policylib::gui::Container1D<policylib::gui::Tab>*>( TabLayout ) );
        parent.appendChild( node );
        return node;
    }
    auto Tab = dynamic_cast<policylib::gui::Tab*>( branch );
    if( Tab ) {
        QDomElement node = dom->createElement( "Tab" );
        addElementAttributes( dom, node, branch );
        addNode( dom, node, Tab->child() );
        parent.appendChild( node );
        return node;
    }
    auto Grid = dynamic_cast<policylib::gui::Grid*>( branch );
    if( Grid ) {
        QDomElement node = dom->createElement( "Grid" );
        addElementAttributes( dom, node, branch );
        addChildren( dom, node, static_cast<policylib::gui::Container2D<policylib::gui::Element>*>( Grid ) );
        parent.appendChild( node );
        return node;
    }
    auto TextInput = dynamic_cast<policylib::gui::TextInput*>( branch );
    if( TextInput ) {
        QDomElement node = dom->createElement( "TextInput" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto Canvas = dynamic_cast<policylib::gui::Canvas*>( branch );
    if( Canvas ) {
        QDomElement node = dom->createElement( "Canvas" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto Label = dynamic_cast<policylib::gui::Label*>( branch );
    if( Label ) {
        QDomElement node = dom->createElement( "Label" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto ComboBox = dynamic_cast<policylib::gui::ComboBox*>( branch );
    if( ComboBox ) {
        QDomElement node = dom->createElement( "ComboBox" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto ElementGroup = dynamic_cast<policylib::gui::ElementGroup*>( branch );
    if( ElementGroup ) {
        QDomElement node = dom->createElement( "ElementGroup" );
        addElementAttributes( dom, node, branch );
        addNode( dom, node, ElementGroup->child() );
        parent.appendChild( node );
        return node;
    }
    auto RadioButtons = dynamic_cast<policylib::gui::RadioButtons*>( branch );
    if( RadioButtons ) {
        QDomElement node = dom->createElement( "RadioButtons" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto SpinBox = dynamic_cast<policylib::gui::SpinBox*>( branch );
    if( SpinBox ) {
        QDomElement node = dom->createElement( "SpinBox" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto CheckBox = dynamic_cast<policylib::gui::CheckBox*>( branch );
    if( CheckBox ) {
        QDomElement node = dom->createElement( "CheckBox" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto Button = dynamic_cast<policylib::gui::Button*>( branch );
    if( Button ) {
        QDomElement node = dom->createElement( "Button" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto HorizontalSlider = dynamic_cast<policylib::gui::HorizontalSlider*>( branch );
    if( HorizontalSlider ) {
        QDomElement node = dom->createElement( "HorizontalSlider" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto HorizontalLayout = dynamic_cast<policylib::gui::HorizontalLayout*>( branch );
    if( HorizontalLayout ) {
        QDomElement node = dom->createElement( "HorizontalLayout" );
        addElementAttributes( dom, node, branch );
        addChildren<policylib::gui::Element>( dom, node, static_cast<policylib::gui::Container1D<policylib::gui::Element>*>( HorizontalLayout ) );
        parent.appendChild( node );
        return node;
    }
    auto VerticalLayout = dynamic_cast<policylib::gui::VerticalLayout*>( branch );
    if( VerticalLayout ) {
        QDomElement node = dom->createElement( "VerticalLayout" );
        addElementAttributes( dom, node, branch );
        addChildren<policylib::gui::Element>( dom, node, static_cast<policylib::gui::Container1D<policylib::gui::Element>*>( VerticalLayout ) );
        parent.appendChild( node );
        return node;
    }
    auto DoubleSpinBox = dynamic_cast<policylib::gui::DoubleSpinBox*>( branch );
    if( DoubleSpinBox ) {
        QDomElement node = dom->createElement( "DoubleSpinBox" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto VerticalSpace = dynamic_cast<policylib::gui::VerticalSpace*>( branch );
    if( VerticalSpace ) {
        QDomElement node = dom->createElement( "VerticalSpace" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto VerticalExpandingSpace = dynamic_cast<policylib::gui::VerticalExpandingSpace*>( branch );
    if( VerticalExpandingSpace ) {
        QDomElement node = dom->createElement( "VerticalExpandingSpace" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto HorizontalSpace = dynamic_cast<policylib::gui::HorizontalSpace*>( branch );
    if( HorizontalSpace ) {
        QDomElement node = dom->createElement( "HorizontalSpace" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    auto HorizontalExpandingSpace = dynamic_cast<policylib::gui::HorizontalExpandingSpace*>( branch );
    if( HorizontalExpandingSpace ) {
        QDomElement node = dom->createElement( "HorizontalExpandingSpace" );
        addElementAttributes( dom, node, branch );
        parent.appendChild( node );
        return node;
    }
    QDomElement node = dom->createElement( "IllegalShouldThrow" );
    parent.appendChild( node );
    return node;
}





}// namespace qtobserver