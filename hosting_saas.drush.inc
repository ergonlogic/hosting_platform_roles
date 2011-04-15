<?php
// $Id$

/**
 * @file
 *
 *   All the heavy lifting
 */

/**
 * Implementation of drush_hook_pre_hosting_task().
 *
 * Override the client in create_admin_user, if SaaS implemented on the platform
 */

function drush_hosting_saas_pre_hosting_task() {
  $task =& drush_get_context('HOSTING_TASK');
  $platform_nid = $task->ref->platform;
  $profile_nid = $task->ref->profile;

  // Here we get the name of the user we want to assign uid1 to.
  // It's in a variable built up using the platform and profile node ID's.
  $uid1_name = variable_get('hosting_saas_' . $platform_nid . '_' . $profile_nid . '_uid1', '');

  // We only need to do this on site installs, and probably not on verify's
  if ($task->ref->type == 'site' && ($task->task_type == 'install' || $task->task_type == 'verify')) {
    $user = user_load(array(name => $uid1_name));
    $task->uid = $user->uid;
  }
}


/*
 * Implementation of hook_post_hosting_TASK_TYPE_task().
 *
 * Here we add a second user to the site, and assign them a role
 */

function hosting_saas_post_hosting_install_task($task, $data) {
  $platform_nid = $task->ref->platform;
  $profile_nid = $task->ref->profile;

  $role = variable_get('hosting_saas_' . $platform_nid . '_' . $profile_nid . '_role', '');
  $uid1 = variable_get('hosting_saas_' . $platform_nid . '_' . $profile_nid . '_uid1', '');

  $saas_client_name = $task->ref->name;
  $saas_user = user_load(array(name => $saas_client_name));
  $saas_client_email = $saas_user->mail;

  // call a backend task to do the actual installation.
  $target = $task->context_options['uri'];
  $commands = array();
  if ($uid1 != '') {
    $commands['user-create'] = 'user-create ' . $saas_client_name . ' --mail=' . $saas_client_email . ' --notify --message=status_activated';
    $commands['user-login'] = 'user-login ' . $saas_client_name;
  }
  if ($role != '') {
    $commands['user-add-role'] = 'user-add-role ' . $role . $saas_client_name;
  }
  // add hook to allow for additional commands?
  $results = array();
  foreach ($commands as $cmd => $command) {
    $result = provision_backend_invoke($target, $command);
    $results[$cmd]['output'] = $result['output'];
  }


  // ref.: hosting_site_post_hosting_login_reset_task()
  $cache = array(
    'expire' => strtotime("+24 hours"), 
    'link' => $results['user-login']['output'],
  );
  cache_set('hosting:site:' . $task->ref->nid . ':login_link', $cache, 'cache', $cache['expire'] );

}
