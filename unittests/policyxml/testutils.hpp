#ifndef TESTUTILS_HPP
#define TESTUTILS_HPP

#include "tinia/policyxml/XMLHandler.hpp"
#include "libxml/tree.h"

struct TestHelper {
    template<class T>
    TestHelper(tinia::policyxml::XMLHandler& handler , T t, bool printDoc = false ) {
        doc = handler.getCompleteDocument();
        if ( printDoc ) {
            xmlSaveFormatFile( "-", doc, 1 );
        }
        t( doc, doc->children );
    }

    ~TestHelper() {
        xmlFreeDoc( doc );

    }

    xmlDocPtr doc;
};

#endif // TESTUTILS_HPP
