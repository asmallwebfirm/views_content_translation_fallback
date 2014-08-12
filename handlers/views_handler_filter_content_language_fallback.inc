<?php

/**
 * @file
 * Contains views_handler_filter_content_language_fallback
 */

class views_handler_filter_content_language_fallback extends views_handler_filter {

  /**
   * A string denoting the fact that the default site language should be used,
   * rather than a specific, configured language.
   */
  const DEFAULT_LANG = '***SITE_DEFAULT_LANGUAGE***';

  /**
   * This filter does not require an operator.
   */
  var $no_operator = TRUE;

  function admin_summary() {
    $languages = $this->languageOptions();
    return $languages[$this->options['fallback_language']];
  }

  function option_definition() {
    $options['fallback_language'] = array(
      'default' => self::DEFAULT_LANG,
      'export' => 'export_option_always',
      'translatable' => FALSE,
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['fallback_language'] = array(
      '#title' => t('Fallback language'),
      '#description' => t('If a piece of content is translated, but not into the current language, the '),
      '#type' => 'select',
      '#options' => $this->languageOptions(),
      '#default_value' => $this->options['fallback_language'],
    );
  }

  /**
   * Do not allow this filter to be exposed.
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * Apply our custom content translation language fallback join and condition.
   */
  function query() {
    $fallback = $this->options['fallback_language'];

    if ($fallback === self::DEFAULT_LANG) {
      $fallback = language_default('language');
    }

    // Add a LEFT JOIN statement against the node table.
    $join_alias = 'ntf';
    $join = new views_join();
    $join->construct('node', 'node', 'nid', 'tnid', "{$join_alias}.language = '{$GLOBALS['language']->language}'", 'LEFT');
    $this->query->add_relationship($join_alias, $join, 'node');

    // Add language filters, allowing for fallback to the site default language.
    $where = "((node.language IN ('{$GLOBALS['language']->language}', '" . LANGUAGE_NONE . "')) OR (node.language IN ('{$fallback}') AND {$join_alias}.nid IS NULL))";
    $this->query->add_where_expression($join_alias, $where);
  }

  /**
   * Returns an array of language options, keyed by language code.
   *
   * @return array
   */
  protected function languageOptions() {
    $languages = array_map(array($this, 'mapLanguages'), language_list());
    $languages = array(self::DEFAULT_LANG => t('Site default language')) + $languages;
    return $languages;
  }

  /**
   * Given a language object, returns the language's name.
   *
   * @param object $language
   *   A Drupal language object.
   * @return string
   *   The name of the language.
   */
  protected function mapLanguages($language) {
    return $language->name;
  }

}
