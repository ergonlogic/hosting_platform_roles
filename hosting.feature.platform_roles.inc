<?php
/**
 * @file
 *    Defines a feature that enables roles and UID1 for site installs to be set
 *    per platform.
 */

/**
 * We need to register a hosting feature with Aegir.
 *
 * @return
 *  associative array indexed by feature key.
 */
function hosting_platform_roles_hosting_feature() {
  $features['platform roles'] = array(
    'title' => t('Platform roles feature'),
    'description' => t('A feature that alters the site install process to assign the client\'s user a role, and not assign them to UID1.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_platform_roles',
    'node' => 'platform',
    'group' => 'experimental'
    );
  return $features;
}
