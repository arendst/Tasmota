/********************************************************************
 * Berry module `mdns`
 * 
 * To use: `import mdns`
 * 
 * MDNS support
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"
#include "be_mem.h"

#ifdef USE_DISCOVERY

extern void m_mdns_start(struct bvm *vm, const char* hostname);
BE_FUNC_CTYPE_DECLARE(m_mdns_start, "", "@[s]")

extern void m_mdns_stop(void);
BE_FUNC_CTYPE_DECLARE(m_mdns_stop, "", "")

extern void m_mdns_set_hostname(struct bvm *vm, const char * hostname);
BE_FUNC_CTYPE_DECLARE(m_mdns_set_hostname, "", "@s")

extern int m_mdns_add_service(struct bvm *vm);
extern int m_mdns_remove_service(struct bvm *vm);
extern int m_dns_add_subtype(struct bvm *vm);
extern int m_dns_add_hostname(struct bvm *vm);
extern int m_dns_find_service(struct bvm *vm);

/* @const_object_info_begin
module mdns (scope: global) {
    start, ctype_func(m_mdns_start)
    stop, ctype_func(m_mdns_stop)
    set_hostname, ctype_func(m_mdns_set_hostname)
    add_service, func(m_mdns_add_service)
    add_hostname, func(m_dns_add_hostname)
    add_subtype, func(m_dns_add_subtype)
    remove_service, func(m_mdns_remove_service)

    // querying
    find_service, func(m_dns_find_service)
}
@const_object_info_end */
#include "be_fixed_mdns.h"

#endif // USE_DISCOVERY