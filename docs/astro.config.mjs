// @ts-check
import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';
import starlightThemeNova from 'starlight-theme-nova';

// https://astro.build/config
export default defineConfig({
	site: 'https://sibkru.github.io',
	base: '/allure-cpp',
	trailingSlash: 'always',
	integrations: [
		starlight({
			title: 'Allure C++',
			social: [{ icon: 'github', label: 'GitHub', href: 'https://github.com/sibkru/allure-cpp' }],
			sidebar: [
				{
					label: 'Guides',
					items: [
						{ label: 'Getting Started', slug: 'guides/getting-started'},
						{ label: 'Setup with GoogleTest', slug: 'guides/setup-googletest'},
						{ label: 'Setup with CppUTest', slug: 'guides/setup-cpputest'},
						{ label: 'Annotate your Testcases', slug: 'guides/annotate-testcases'},
						{ label: 'Add a Testing Framework', slug: 'guides/add-framework'},
					],
				},
				{
					label: 'Example Reports',
					items: [
						{ label: 'GoogleTest Example Report', link: '/googletest-example' },
						{ label: 'CppUTest Example Report', link: '/cpputest-example' },
					],
				},
				{
					label: 'Reference',
					autogenerate: { directory: 'reference' },
				},
			],
			plugins: [
        		starlightThemeNova(/* options */),
      		]
		}),
	],
});
