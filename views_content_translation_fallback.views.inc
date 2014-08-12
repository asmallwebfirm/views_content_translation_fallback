<?php

/**
 * @file
 * Views hooks and functions for the Views Content Translation Fallback module.
 */


/**
 * Implements hook_views_data_alter().
 */
function views_content_translation_fallback_views_data_alter(&$data) {
  $data['node']['language_with_fallback'] = array(
    'title' => t('Fallback'),
    'group' => t('Content translation'),
    'filter' => array(
      'handler' => 'views_handler_filter_content_language_fallback',
      'label' => t('Language with fallback'),
    ),
  );
}
