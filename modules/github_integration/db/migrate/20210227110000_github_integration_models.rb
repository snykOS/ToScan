class GithubIntegrationModels < ActiveRecord::Migration[6.0]
  def change
    # see https://docs.github.com/en/rest/reference/pulls
    create_table "github_pull_request" do |t|
      t.references :github_user, null: false

      t.integer :github_id, null: false, index: true
      t.integer :number, null: false
      t.string :github_html_url, null: false
      t.datetime :github_updated_at, null: false
      t.string :state, null: false
      t.string :title, null: false
      t.text :body, null: false
      t.boolean :draft, null: false
      t.boolean :merged, null: false
      t.string :merged_by
      t.datetime :merged_at
      t.integer :comments_count, null: false
      t.integer :review_comments_count, null: false
      t.integer :additions_count, null: false
      t.integer :deletions_count, null: false
      t.integer :changed_files_count, null: false
      t.json :labels, null: false # [{name, description, color}]
      t.timestamps
    end

    create_table "github_pull_requests_work_packages", if: false do |t|
      t.references :github_pull_request, null: false, index: {
        # the default index name was too long
        name: "index_github_pull_requests_work_packages_on_github_pr_id"
      }
      t.references :work_package, null: false
    end

    # see: https://docs.github.com/en/rest/reference/users
    create_table :github_users do |t|
      t.references :user, null: true

      t.integer :github_id, null: false, index: true
      t.string :github_login, null: false
      t.string :github_url, null: false
      t.string :github_avatar_url, null: false

      t.timestamps
    end

    # see: https://docs.github.com/en/rest/reference/checks
    create_table :github_check_runs do |t|
      t.references :github_pull_request, null: false

      t.integer :github_id, null: false, index: true
      t.string :github_html_url, null: false
      t.string :github_app_owner_avatar_url, null: false
      t.string :status, null: false
      t.string :conclusion
      t.string :output_title
      t.string :output_summary
      t.string :details_url
      t.datetime :started_at
      t.datetime :completed_at

      t.timestamps
    end
  end
end
