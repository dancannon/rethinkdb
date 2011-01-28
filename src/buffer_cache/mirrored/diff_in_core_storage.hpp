#ifndef __DIFF_IN_CORE_STORAGE_HPP__
#define	__DIFF_IN_CORE_STORAGE_HPP__

#include <list>
#include <map>
#include "buffer_cache/buf_patch.hpp"

// TODO: Maybe move this file out of mirrored?

class diff_core_storage_t {
public:
    diff_core_storage_t(); // Initialize an empty diff storage

    // This assumes that patches is properly sorted. It will initialize a block_patch_list_t and store it.
    void load_block_patch_list(const block_id_t block_id, const std::list<buf_patch_t*>& patches);

    // Removes all patches up to (including) the given patch id
    void truncate_applied_patches(const block_id_t block_id, const patch_counter_t patch_counter);

    // Returns true iff any changes have been made to the buf
    bool apply_patches(const block_id_t block_id, char *buf_data) const;

    void store_patch(const block_id_t block_id, buf_patch_t &patch);

    // Return NULL if no patches exist for that block
    const std::list<buf_patch_t*>* get_patches(const block_id_t block_id) const;

    // TODO! Change the meaning of the dirty flag in buf_t! Introduce a needs_flush flag!
    // Remove all patches for that block (e.g. after patches have been applied and the block gets flushed to disk)
    void drop_patches(const block_id_t block_id);

private:
    struct block_patch_list_t : public std::list<buf_patch_t*> {
        ~block_patch_list_t(); // Deletes all stored patches
        void truncate_up_to_patch(const patch_counter_t patch_counter); // Deletes all patches up to (including) the given patch_counter
    };

    typedef std::map<block_id_t, block_patch_list_t> patch_map_t;
    patch_map_t patch_map;
};


#endif	/* __DIFF_IN_CORE_STORAGE_HPP__ */

