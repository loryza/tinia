
#include "Tutorial3_Job.hpp"
/** [headers] */
#include "tinia/trell/IPCGLJobController.hpp"
/** [headers] */

/** [main] */
int main(int argc, char** argv) {
    /** [job] */
    tinia::tutorial::Tutorial3Job job;
    /** [job] */

    /** [controller] */
    tinia::trell::IPCGLJobController controller;
    /** [controller] */

    /** [jobtocontroller] */
    controller.setJob(&job);
    /** [jobtocontroller] */

    /** [run]*/
    return controller.run(argc, argv);
    /** [run]*/
}
/** [main] */

