<?php
// $Id$

/**
 * @file
 *
 *   These are the backend commands that actually accomplish adding a second
 *   user, adds the drush command to notify them of it, assigns them a role and
 *   updates the login reset link.
 */

/**
 * Implementation of drush_hook_pre_hosting_task().
 *
 * Overrides the user that'll be used during install in create_admin_user()
 */
function drush_hosting_platform_roles_pre_hosting_task() {
  $task =& drush_get_context('HOSTING_TASK');

  if ($task->ref->type == 'site' && $task->task_type == 'install') {
    if ($uid1 = db_result(db_query('SELECT {uid} FROM {hosting_platform_roles_uid1} WHERE pl_nid=%d AND pr_nid=%d', $task->ref->platform, $task->ref->profile))) {
      $task->uid = user_load(array('name' => $uid1))->uid;
    }
  }
}

/*
 * Implementation of hook_post_hosting_TASK_TYPE_task().
 *
 * Runs the backend commands to create a second user, send a notification,
 * update the login link, and assign a role. 
 */
function hosting_platform_roles_post_hosting_install_task($task, $data) {
  $role = db_result(db_query('SELECT {role} FROM {hosting_platform_roles_roles} WHERE pl_nid=%d AND pr_nid=%d', $task->ref->platform, $task->ref->profile));
  $uid1 = db_result(db_query('SELECT {uid} FROM {hosting_platform_roles_uid1} WHERE pl_nid=%d AND pr_nid=%d', $task->ref->platform, $task->ref->profile));

  $client_name = $task->ref->name;
  $target = $task->context_options['uri'];
  if ($uid1 != FALSE) {

    // if http://drupal.org/node/1116414 gets committed, we can remove the "notify" command code
    $results['user-create'] = provision_backend_invoke($target, 'user-create ' . $client_name . ' --mail=' . user_load(array('name' => $client_name))->mail);
    drush_log(dt('User created:') . $results['user-create']['output']);
    
    $results['notify'] = provision_backend_invoke($target, 'notify ' . $client_name);
    drush_log($results['notify']);
    
    $results['user-login'] = provision_backend_invoke($target, 'user-login ' . $client_name);
    drush_log(dt('Client user login link: ') . $results['user-login']['output']);
    
    // Set the site login link to the one we just created; ref.: hosting_site_post_hosting_login_reset_task()
    $cache = array(
      'expire' => strtotime("+24 hours"), 
      'link' => $results['user-login']['output'],
    );
    cache_set('hosting:site:' . $task->ref->nid . ':login_link', $cache, 'cache', $cache['expire'] );
  }
  else {
    // we may want to do this for block-placement, etc.
    $result = provision_backend_invoke($target, 'eval "print(user_load(1)->name)"');
    $client_name = $result['output'];
  }
  
  if ($role != FALSE) {
    // TODO: support adding multiple roles
    // we can finally check that the role actually exists on this site
    $result = provision_backend_invoke($target, 'eval "print(serialize(user_roles()))"');
    $site_roles = unserialize($result['output']);
    if (in_array($role, $site_roles)) {
      // we need to avoid a role name like ";rm -rf /", which is a valid role name in D6, btw
      drush_escapeshellarg(&$role);
      provision_backend_invoke($target, 'user-add-role ' . $role . ' --name=' . $client_name);
    }
    else {
      drush_set_error(DRUSH_APPLICATION_ERROR, dt('No role of <em>' . $role . '</em> is available on this platform'));
    }
  }
}

/**
 * Implementation of hook_drush_command().
 */
function hosting_platform_roles_drush_command() {
  $items['notify'] = array(
    'callback' => 'drush_notify',
    // Why drush_notify() instead of drush_hosting_platform_roles_notify()?
    // That gets run multiple times. See: http://drupal.org/node/704848
    // Should be fixed in Drush > 4.4
    'aliases' => array('notify'),
    'description' => dt('Send email to client notifying of the creation of their new user account.'),
    'arguments' => array(
      'name' => dt('The name of the account to send the notification to.'),
    ),
    'options' => array(
      'message' => dt('The message to send.
    allowed values: - "register_admin_created (default)"
                    - "register_no_approval_required"
                    - "register_pending_approval"
                    - "status_activated"'),
    ),
  );
  return $items;
}

/**
 * Drush command to send notification emails
 */
function drush_notify($name) {
  $message = drush_get_option('message');
  $message_options = array(
    'register_admin_created' => 'Welcome message for user created by the admin',
    'register_no_approval_required' => 'Welcome message when user self-registers',
    'register_pending_approval' => 'Welcome message, user pending admin approval',
    'status_activated' => 'Account activated',
  );
  if ($message == FALSE) {
    // set a default message
    $message = 'status_activated';
  }
  elseif ($message === TRUE) {
    $message = drush_choice($message_options, 'Enter a number to choose which message to send.', '!value (!key)');
  }
  elseif (!array_key_exists($message, $message_options)) {
    return drush_set_error($message . ' is not a valid message!');
  }
  _user_mail_notify($message, user_load(array('name' => $name)));
  drush_print(' Sent notification: ' . $message_options[$message] . ' (' . $message . ')');
}
