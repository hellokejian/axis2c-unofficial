/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <rp_saml_token_builder.h>

axis2_status_t AXIS2_CALL saml_token_process_alternatives(
    const axutil_env_t *env,
    neethi_all_t *all,
    rp_saml_token_t *saml_token);

AXIS2_EXTERN neethi_assertion_t *AXIS2_CALL
    rp_saml_token_builder_build(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
{
    rp_saml_token_t *saml_token = NULL;
    axis2_char_t *inclusion_value = NULL;
    neethi_policy_t *policy = NULL;
    axiom_node_t *child_node = NULL;
    axiom_element_t *child_element = NULL;
    axutil_array_list_t *alternatives = NULL;
    neethi_operator_t *component = NULL;
    neethi_all_t *all = NULL;
    axutil_qname_t *qname = NULL;
    neethi_assertion_t *assertion = NULL;
    neethi_policy_t *normalized_policy = NULL;
    
    saml_token = rp_saml_token_create(env);
    qname = axutil_qname_create(env, RP_INCLUDE_TOKEN, RP_SP_NS_11, RP_SP_PREFIX);
    inclusion_value = axiom_element_get_attribute_value(element, env, qname);
    axutil_qname_free(qname, env);
    qname = NULL;

    if(!inclusion_value)
    {
        /* we can try whether WS-SP1.2 specific inclusion value */
        qname = axutil_qname_create(env, RP_INCLUDE_TOKEN, RP_SP_NS_12, RP_SP_PREFIX);
        inclusion_value = axiom_element_get_attribute_value(element, env, qname);
        axutil_qname_free(qname, env);
        qname = NULL;
    }

    rp_saml_token_set_inclusion(saml_token, env, inclusion_value);
    
    child_node = axiom_node_get_first_element(node, env);
    if (!child_node)
    {
        assertion = neethi_assertion_create(env);
        neethi_assertion_set_value(assertion, env,
                                   saml_token,
                                   ASSERTION_TYPE_SAML_TOKEN);
        return assertion;
    }
    if (axiom_node_get_node_type(child_node, env) == AXIOM_ELEMENT)
    {
        child_element =
            (axiom_element_t *) axiom_node_get_data_element(child_node, env);
        if (child_element)
        {
            policy = neethi_engine_get_policy(env, child_node, child_element);
            if (!policy)
            {
                return NULL;
            }
            normalized_policy =
                neethi_engine_get_normalize(env, AXIS2_FALSE, policy);
            neethi_policy_free(policy, env);
            policy = NULL;
            alternatives =
                neethi_policy_get_alternatives(normalized_policy, env);
            component =
                (neethi_operator_t *) axutil_array_list_get(alternatives, env,
                                                            0);
            all = (neethi_all_t *) neethi_operator_get_value(component, env);
            saml_token_process_alternatives(env, all, saml_token);

            assertion =
                neethi_assertion_create_with_args(env,
                                                  (AXIS2_FREE_VOID_ARG)rp_saml_token_free,
                                                  saml_token,
                                                  ASSERTION_TYPE_SAML_TOKEN);

            neethi_policy_free(normalized_policy, env);
            normalized_policy = NULL;

            return assertion;
        }
        else
            return NULL;
    }
    else
        return NULL;
}

axis2_status_t AXIS2_CALL saml_token_process_alternatives(
    const axutil_env_t *env,
    neethi_all_t *all,
    rp_saml_token_t *saml_token)
{
    neethi_operator_t *operator = NULL;
    axutil_array_list_t *arraylist = NULL;
    neethi_assertion_t *assertion = NULL;
    neethi_assertion_type_t type;

    int i = 0;

    arraylist = neethi_all_get_policy_components(all, env);

    for (i = 0; i < axutil_array_list_size(arraylist, env); i++)
    {
        operator =(neethi_operator_t *) axutil_array_list_get(arraylist, env,
                                                              i);
        assertion =
            (neethi_assertion_t *) neethi_operator_get_value(operator, env);
        type = neethi_assertion_get_type(assertion, env);
        
        if(type == ASSERTION_TYPE_REQUIRE_DERIVED_KEYS_SC10)
        {
            rp_saml_token_set_derivedkeys(saml_token, env, AXIS2_TRUE);
        }
        else if (type == ASSERTION_TYPE_REQUIRE_KEY_IDENTIFIRE_REFERENCE)
        {
            rp_saml_token_set_require_key_identifier_reference(saml_token, env,
                                                               AXIS2_TRUE);
        }
        else if(type == ASSERTION_TYPE_WSS_SAML_V10_TOKEN_V10)
        {
            rp_saml_token_set_token_version_and_type(saml_token, env, RP_WSS_SAML_V10_TOKEN_V10);
        }
        else if(type == ASSERTION_TYPE_WSS_SAML_V10_TOKEN_V11)
        {
            rp_saml_token_set_token_version_and_type(saml_token, env, RP_WSS_SAML_V10_TOKEN_V11);
        }
        else if(type == ASSERTION_TYPE_WSS_SAML_V11_TOKEN_V10)
        {
            rp_saml_token_set_token_version_and_type(saml_token, env,RP_WSS_SAML_V11_TOKEN_V10);
        }
        else if(type == ASSERTION_TYPE_WSS_SAML_V11_TOKEN_V11)
        {
            rp_saml_token_set_token_version_and_type(saml_token, env, RP_WSS_SAML_V11_TOKEN_V11);
        }
        else if(type == ASSERTION_TYPE_WSS_SAML_V20_TOKEN_V11)
        {
            rp_saml_token_set_token_version_and_type(saml_token, env, RP_WSS_SAML_V20_TOKEN_V11);
        }
        else
            return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

