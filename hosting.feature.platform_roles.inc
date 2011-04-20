<?php
// $Id$
/**
 * @file
 *    Defines a feature that enables roles and UID1 for site installs to be set
 *    per platform.
 */

/**
 * We need to register a hosting feature with Aegir.
 *
 * This is used to generate the 'admin/hosting/platform_roles' page.
 *
 * @return
 *  associative array indexed by feature key.
 */
function hosting_platform_roles_hosting_feature() {
  $features['platform roles'] = array(
    // title to display in form
    'title' => t('Platform roles feature'),
    // description
    'description' => t('A feature that alters the site install process to assign the client\'s user a role, and not assign them to UID1.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_platform_roles',
    // associate with a specific node type.
    'node' => 'platform',
    // which group to display in ( null , experimental )
    'group' => 'experimental'
    );
  return $features;
}
