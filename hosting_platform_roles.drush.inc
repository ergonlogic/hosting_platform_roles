<?php
// $Id$

/**
 * @file
 *
 *   Here we hook into the site install task to the change the user prior to 
 *   install, and then add a second user for the client, and assign them a role.
 */

/**
 * Implementation of drush_hook_pre_hosting_task().
 *
 * Override the user that'll be used in create_admin_user()
 */

function drush_hosting_platform_roles_pre_hosting_task() {
  $task =& drush_get_context('HOSTING_TASK');

  if ($task->ref->type == 'site' && ($task->task_type == 'install')) {
    $task->uid = user_load(db_result(db_query('SELECT {uid} FROM {hosting_platform_roles_uid1} WHERE pl_nid=%d AND pr_nid=%d', $task->ref->platform, $task->ref->profile)))->uid;

  }
}


/*
 * Implementation of hook_post_hosting_TASK_TYPE_task().
 *
 * Here we add a second user to the site, and assign them a role
 */

function hosting_platform_roles_post_hosting_install_task($task, $data) {
  $role = db_result(db_query('SELECT {role} FROM {hosting_platform_roles_roles} WHERE pl_nid=%d AND pr_nid=%d', $task->ref->platform, $task->ref->profile));
  $uid1 = db_result(db_query('SELECT {uid} FROM {hosting_platform_roles_uid1} WHERE pl_nid=%d AND pr_nid=%d', $task->ref->platform, $task->ref->profile));

  $client_name = $task->ref->name;
  $client_email = user_load(array('name' => $client_name))->mail;

  // call a backend task to do the actual installation.
  $target = $task->context_options['uri'];
  $commands = array();
  if ($uid1 != FALSE) {
    $commands['user-create'] = 'user-create ' . $client_name . ' --mail=' . $client_email . ' --notify --message=status_activated';
    $commands['user-login'] = 'user-login ' . $client_name;
  }
  if ($role != '') {
    $commands['user-add-role'] = 'user-add-role ' . $role . ' --name=' . $client_name;

  }
  // add hook to allow for additional commands?
  $results = array();
  foreach ($commands as $cmd => $command) {
    $result = provision_backend_invoke($target, $command);
    $results[$cmd]['output'] = $result['output'];
  }

  // Set the site login link to the one generate post install for uid3
  // ref.: hosting_site_post_hosting_login_reset_task()
  $cache = array(
    'expire' => strtotime("+24 hours"), 
    'link' => $results['user-login']['output'],
  );
  cache_set('hosting:site:' . $task->ref->nid . ':login_link', $cache, 'cache', $cache['expire'] );

}
