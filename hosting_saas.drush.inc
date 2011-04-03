<?php

/**
 * Implementation of drush_hook_pre_hosting_task()
 *
 * Override the client in create_admin_user, if SaaS implemented on the profile/platform
 */
/*
function drush_hosting_saas_pre_hosting_task() {
  $task =& drush_get_context('HOSTING_TASK');
  if ($task->ref->type == 'site' && ($task->task_type == 'install' || $task->task_type == 'verify')) {
    $task->options['cheese'] = $task->ref->cheese;
  }
}
*/

/**
 * Implementaion of hook_post_hosting_TASK_TYPE_task()
 *
 * Here we add a second user to the site, and assign them a role
 */
function hosting_saas_post_hosting_install_task($task, $data) {

  $platform_nid = $task->ref->platform;
  $profile_nid = $task->ref->profile;

  $role = variable_get('hosting_saas_' . $platform_nid . '_' . $profile_nid . '_role', '');
  $uid1 = variable_get('hosting_saas_' . $platform_nid . '_' . $profile_nid . '_uid1', '');

  $client_name = 'test-user'; //$task->ref->name;
  $client_email = 'test@ergonlogic.com'; //$task->options['client_email'];

  //drush_bootstrap(DRUSH_BOOTSTRAP_DRUPAL_SITE);
  // call a backend task to do the actual installation.
  $target = $task->context_options['uri'];
  $commands = array();
  if ($uid1 != '') {
    $commands[] = 'user-create ' . $client_name . ' --mail=' . $client_email;
  }
  if ($role != '') {
    $commands[] = 'user-add-role ' . $role . $client_name;
  }
  // add hook to allow for additional commands?
  foreach ($commands as $command) {
    provision_backend_invoke($target, $command);
  }
  // pass the login link back to the front end.
  //  drush_set_option('login_link', $result['context']['login_link']);
}
