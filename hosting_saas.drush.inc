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

  $uid1_name = variable_get('hosting_saas_' . $platform_nid . '_' . $profile_nid . '_uid1', '');

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
    $commands[] = 'user-create ' . $saas_client_name . ' --mail=' . $saas_client_email;
  }
  if ($role != '') {
    $commands[] = 'user-add-role ' . $role . $saas_client_name;
  }
  // add hook to allow for additional commands?
  foreach ($commands as $command) {
    provision_backend_invoke($target, $command);
  }
  // pass the login link back to the front end.
  //  drush_set_option('login_link', $result['context']['login_link']);
}
