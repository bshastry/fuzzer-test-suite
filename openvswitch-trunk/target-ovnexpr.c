#include "openvswitch/dynamic-string.h"
#include "ovn/expr.h"
#include "openvswitch/shash.h"
#include "simap.h"
#include "flow.h"
#include "ovn/lib/logical-fields.h"

static const int steps = 4;

static bool
lookup_port_cb(const void *ports_, const char *port_name, unsigned int *portp)
{
    const struct simap *ports = ports_;
    const struct simap_node *node = simap_find(ports, port_name);
    if (!node) {
        return false;
    }
    *portp = node->data;
    return true;
}

static bool
is_chassis_resident_cb(const void *ports_, const char *port_name)
{
    const struct simap *ports = ports_;
    const struct simap_node *node = simap_find(ports, port_name);
    if (node) {
      return true;
    }
    return false;
}

static void
create_symtab(struct shash *symtab)
{
    ovn_init_symtab(symtab);

    /* For negative testing. */
    expr_symtab_add_field(symtab, "bad_prereq", MFF_XREG0, "xyzzy", false);
    expr_symtab_add_field(symtab, "self_recurse", MFF_XREG0,
                          "self_recurse != 0", false);
    expr_symtab_add_field(symtab, "mutual_recurse_1", MFF_XREG0,
                          "mutual_recurse_2 != 0", false);
    expr_symtab_add_field(symtab, "mutual_recurse_2", MFF_XREG0,
                          "mutual_recurse_1 != 0", false);
    expr_symtab_add_string(symtab, "big_string", MFF_XREG0, NULL);
}

static void
create_addr_sets(struct shash *addr_sets)
{
    shash_init(addr_sets);

    static const char *const addrs1[] = {
        "10.0.0.1", "10.0.0.2", "10.0.0.3",
    };
    static const char *const addrs2[] = {
        "::1", "::2", "::3",
    };
    static const char *const addrs3[] = {
        "00:00:00:00:00:01", "00:00:00:00:00:02", "00:00:00:00:00:03",
    };
    static const char *const addrs4[] = { NULL };

    expr_addr_sets_add(addr_sets, "set1", addrs1, 3);
    expr_addr_sets_add(addr_sets, "set2", addrs2, 3);
    expr_addr_sets_add(addr_sets, "set3", addrs3, 3);
    expr_addr_sets_add(addr_sets, "set4", addrs4, 0);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {

    struct shash symtab;
    struct shash addr_sets;
    struct simap ports;
    struct ds input;

    create_symtab(&symtab);
    create_addr_sets(&addr_sets);

    simap_init(&ports);
    simap_put(&ports, "eth0", 5);
    simap_put(&ports, "eth1", 6);
    simap_put(&ports, "LOCAL", ofp_to_u16(OFPP_LOCAL));

    ds_init(&input);
    ds_put_buffer(&input, (const char *)data, size);
    struct expr *expr;
    char *error;

    expr = expr_parse_string(ds_cstr(&input), &symtab, &addr_sets, &error);
    if (!error && steps > 0) {
        expr = expr_annotate(expr, &symtab, &error);
    }
    if (!error) {
        if (steps > 1) {
            expr = expr_simplify(expr, is_chassis_resident_cb, &ports);
        }
        if (steps > 2) {
            expr = expr_normalize(expr);
            ovs_assert(expr_is_normalized(expr));
        }
    }
    if (!error) {
        if (steps > 3) {
            struct hmap matches;

            expr_to_matches(expr, lookup_port_cb, &ports, &matches);
            expr_matches_print(&matches, stdout);
            expr_matches_destroy(&matches);
        } else {
            struct ds output = DS_EMPTY_INITIALIZER;
            expr_format(expr, &output);
            puts(ds_cstr(&output));
            ds_destroy(&output);
        }
    } else {
        puts(error);
        free(error);
    }
    expr_destroy(expr);
    ds_destroy(&input);

    simap_destroy(&ports);
    expr_symtab_destroy(&symtab);
    shash_destroy(&symtab);
    expr_addr_sets_destroy(&addr_sets);
    shash_destroy(&addr_sets);
    return 0;
}
