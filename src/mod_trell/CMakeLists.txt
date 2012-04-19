SET( MOD_TRELL_SRC
    "mod_trell.c"
    "mod_trell_ops.c"
    "mod_trell_job.c"
    "mod_trell_util.c"
    "mod_trell_reply.c"
    "mod_trell_send_png.c"
    "mod_trell_send_bmp.c"
    "mod_trell_send_xml.c"
    "mod_trell_send_file.c"
    "mod_trell_url_decode.c"
    "../lib_trell/messenger.c"
)

ADD_LIBRARY( mod_trell SHARED ${MOD_TRELL_SRC} )
TARGET_LINK_LIBRARIES( mod_trell ${RT} ${LIBXML2_LIBRARIES} ${LIB_APR} )

INSTALL( TARGETS mod_trell
  LIBRARY DESTINATION /var/trell/module
  PERMISSIONS WORLD_READ WORLD_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE
  COMPONENT mod_trell
)

INSTALL( FILES ../../config/mod_trell.load ../../config/mod_trell.conf
  DESTINATION /etc/apache2/mods-available
  PERMISSIONS WORLD_READ OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  COMPONENT mod_trell
)
INSTALL( DIRECTORY "../../js"
  DESTINATION /var/trell
  COMPONENT mod_trell
  PATTERN ".git" EXCLUDE
)


INSTALL( DIRECTORY ../../static
  DESTINATION /var/trell/
  COMPONENT mod_trell
  PATTERN ".git" EXCLUDE
)

INSTALL( DIRECTORY ../../schemas
  DESTINATION /var/trell/
  COMPONENT mod_trell
  PATTERN ".git" EXCLUDE
)