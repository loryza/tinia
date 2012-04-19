#include <iostream>
#include <librenderlist/Logger.hpp>
#include <librenderlist/Draw.hpp>
#include <librenderlist/SetShader.hpp>
#include <librenderlist/gl/RenderSetShader.hpp>
#include <librenderlist/gl/RenderShader.hpp>
#include <librenderlist/gl/Renderer.hpp>
#include "Utils.hpp"

namespace librenderlist {
namespace gl {

static const std::string package = "librenderlist.gl.RenderSetShader";

RenderSetShader::RenderSetShader( Renderer& renderer, Id id )
    : RenderAction( renderer, id ),
      m_gl_program( 0 )
{}


void
RenderSetShader::pull( const SetShader* a )
{
    static const std::string log = package + ".pull: ";

    const RenderShader* s = m_renderer.shader( a->shaderId() );
    if( s == NULL ) {
        std::cerr << "failed to locate shader id=" << a->shaderId() << std::endl;
        m_gl_program = 0;
    }
    else {
        m_gl_program = s->program();
    }
}

void
RenderSetShader::invoke( RenderState& state )
{
    glUseProgram( m_gl_program );
#ifdef DEBUG
    Logger log = getLogger( package + ".invoke" );
    CHECK_GL;
#endif
}




} // of namespace gl
} // of namespace librenderlist