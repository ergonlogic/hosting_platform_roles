<?php
// $Id$
/**
 * @file
 *    defines a feature that enables some SaaS requirements
 * 
 */

/**
 * We need to register a hosting feature with Aegir.
 *
 * This is used to generate the 'admin/hosting/saas' page.
 * 
 * @return 
 *  associative array indexed by feature key.
 */
function hosting_saas_hosting_feature() {
  $features['saas'] = array(
    // title to display in form
    'title' => t('SaaS feature'),
    // description
    'description' => t('Software-as-a-Service feature tweaking the site install process to not give up UID1.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_saas',
    // associate with a specific node type.
    'node' => 'platform',
    // which group to display in ( null , experimental )
    'group' => 'experimental'
    );
  return $features;
}
